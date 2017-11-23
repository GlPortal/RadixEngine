#include <radix/SimulationManager.hpp>

#include <utility>

#include <radix/env/Util.hpp>
#include <radix/World.hpp>
#include <radix/util/Profiling.hpp>

namespace radix {

SimulationManager::SimulationRunner::SimulationRunner(SimulationManager &sysMan) :
  threads(std::thread::hardware_concurrency()),
  exit(false) {
  for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
    threads.emplace_back(std::bind(&SimulationRunner::threadProc, this, i, std::ref(sysMan)));
  }
}

void SimulationManager::SimulationRunner::threadProc(int i, SimulationManager &simMan) {
  PROFILER_THREAD_SCOPE(("SimRunnr " + std::to_string(i)).c_str());
  Util::SetCurrentThreadName("SimRunnr " + std::to_string(i));
  while (!exit) {
    GraphNode *node;
    { PROFILER_BLOCK("Awaiting queue", profiler::colors::White);
      std::unique_lock<std::mutex> lk(queueMutex);
      queueCondVar.wait(lk, [this](){ return exit || queue.size() > 0; });
      if (exit) {
        return;
      }
      node = queue.front();
      queue.pop();
    }
    PROFILER_BLOCK(node->simulation->getName(), profiler::colors::Cyan200);
    node->simulation->update(dtime);
    PROFILER_END_BLOCK;
    { std::unique_lock<std::mutex> lk(queueMutex);
      for (GraphNode *nextNode : node->successors) {
        unsigned int counter = ++(nextNode->counter);
        if (counter == nextNode->predecessors.size()) {
          queue.push(nextNode);
          queueCondVar.notify_one();
        }
      }
    }
    { std::unique_lock<std::mutex> lk(runCountMutex);
      ++runCount;
      runCountCondVar.notify_all();
    }
  }
}

SimulationManager::SimulationRunner::~SimulationRunner() {
  exit = true;
  queueCondVar.notify_all();
  for (std::thread &thr : threads) {
    if (thr.joinable()) {
      thr.join();
    }
  }
}

SimulationManager::GraphNode::GraphNode(SimulationPtr &&simulation, Graph &graph) :
  simulation(std::move(simulation)),
  graph(graph) {
}

using SimulationGraphNode = SimulationManager::GraphNode;

static void transitiveReduction(SimulationGraphNode &vertex0, SimulationGraphNode &child0,
        std::set<SimulationGraphNode*> &done) {
  if (done.find(&child0) != done.end()) {
    return;
  }
  for (SimulationGraphNode *successor : child0.successors) {
    vertex0.successors.erase(successor);
    successor->predecessors.erase(&vertex0);
    transitiveReduction(vertex0, *successor, done);
  }
  done.insert(&child0);
}

#if 0
static void dumpGraph(const std::string &path, const std::vector<std::unique_ptr<Simulation>> &systems,
  const SimulationGraphNodeVector &sinfo) {
  std::ofstream dot;
  dot.open(path, std::ios_base::out | std::ios_base::trunc);
  dot << "digraph SimulationRunGraph {" << std::endl;
  for (const std::unique_ptr<SimulationGraphNode> &sgnptr : sinfo) {
    if (sgnptr) {
      if (sgnptr->successors.size() > 0) {
        for (SimulationTypeId succStid : sgnptr->successors) {
          dot << systems[sgnptr->system]->getName() << " -> " << systems[succStid]->getName() << ';' << std::endl;
        }
      } else {
        dot << systems[sgnptr->system]->getName() << ';' << std::endl;
      }
    }
  }
  dot << "}";
  dot.close();
  std::system(("dot -O -Tpng " + path).c_str());
}
#endif

void SimulationManager::computeSimulationOrder() {
  // Each Simulation can request to be run before other Simulations, and after some others.
  // This allows for the injection of Simulations between two other in the execution graph, which
  // otherwise wouldn't be possible with only 1 of the "run before" and "run after" condition.
  // Since a "run before" condition is just a reversed "run after", we don't have to handle the
  // two separately.
  // Now, this exhibits a single issue (in our case): creating loops in the graph,
  // which we don't want because it 1. wouldn't make sense and 2. would run forever.
  //
  // A solution to create an execution graph is the following:
  // Step 1: build the tree, checking if insertion would create a loop
  //         (if yes, throw an error)
  // Step 2: delete any edges that can be transitively obtained (i.e. only keeping the longest
  //         paths), a.k.a. transitive reduction

  /* Step 1: O(V²) */ {
    for (GraphNodeRef &node : graph) {
      if (node) {
        for (const GraphNodeRef &otherNode : graph) {
          if (otherNode &&
              otherNode.get() != node.get()) {
            bool nodeRunsBeforeOther = node->simulation->runsBefore(*otherNode->simulation),
                 nodeRunsAfterOther = node->simulation->runsAfter(*otherNode->simulation),
                 otherRunsBeforeNode = otherNode->simulation->runsBefore(*node->simulation),
                 otherRunsAfterNode = otherNode->simulation->runsAfter(*node->simulation);
            // Sanity checks
            if (nodeRunsBeforeOther && nodeRunsAfterOther) {
              throw std::logic_error(
                  std::string("Simulation ") + node->simulation->getName() +
                  " wants to run both before && after " + otherNode->simulation->getName());
            }
            if (otherRunsBeforeNode && otherRunsAfterNode) {
              throw std::logic_error(
                  std::string("Simulation ") + otherNode->simulation->getName() +
                  " wants to run both before && after " + node->simulation->getName());
            }
            if (nodeRunsBeforeOther && otherRunsBeforeNode) {
              throw std::logic_error(
                  std::string("Both ") + node->simulation->getName() + " && " +
                  otherNode->simulation->getName() + " want to run before each other");
            }
            if (nodeRunsAfterOther && otherRunsAfterNode) {
              throw std::logic_error(
                  std::string("Both ") + node->simulation->getName() + " && " +
                  otherNode->simulation->getName() + " want to run after each other");
            }
            // Node -> Other
            if (nodeRunsBeforeOther || otherRunsAfterNode) {
              node->successors.insert(otherNode.get());
              otherNode->predecessors.insert(node.get());
            }
            // Other -> Node
            else if (nodeRunsAfterOther || otherRunsBeforeNode) {
              otherNode->successors.insert(node.get());
              node->predecessors.insert(otherNode.get());
            }
          }
        }
      }
    }
  }

  /* Step 2 */ {
    // https://stackoverflow.com/a/11237184/1616310
    for (GraphNodeRef &node : graph) {
      if (node) {
        std::set<GraphNode*> done;
        for (GraphNode *successor : node->successors) {
          transitiveReduction(*node, *successor, done);
        }
      }
    }
  }
}

SimulationManager::SimulationManager(World &w) :
  w(w),
  systemRun(*this) {
}

void SimulationManager::dispatchEvent(const Event &evt) {
  w.event.dispatch(evt);
}

void SimulationManager::run(TDelta dtime) {
  PROFILER_BLOCK("SimulationManager::run", profiler::colors::Coral);
  std::unique_lock<std::mutex> lk(systemRun.runCountMutex);
  // Reset previous run count and set the dtime parameter to pass to Simulations
  systemRun.runCount = 0;
  systemRun.dtime = dtime;

  // Count the Simulations so we know when all of them were run. Also count the ones without
  // predecessors, i.e. the ones that runs first, to know how many threads to notify.
  unsigned int targetRunCount = 0, startCount = 0;
  { std::unique_lock<std::mutex> qlk(systemRun.queueMutex);
    for (std::unique_ptr<GraphNode> &node : graph) {
      if (node) {
        node->counter = 0;
        if (node->predecessors.empty()) {
          systemRun.queue.push(node.get());
          ++startCount;
        }
        ++targetRunCount;
      }
    }
  }

  // Wake (notify) threads to start running Simulations
  while (startCount--) {
    systemRun.queueCondVar.notify_one();
  }

  // Wait for all Simulations to have run, i.e. runCount reached its target.
  PROFILER_BLOCK("Awaiting threads");
  systemRun.runCountCondVar.wait(lk, [this, targetRunCount]() {
    return systemRun.runCount == targetRunCount;
  });
}

} /* namespace radix */
