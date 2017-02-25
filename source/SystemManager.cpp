#include <radix/SystemManager.hpp>

#include <radix/env/Util.hpp>
#include <radix/World.hpp>

namespace radix {

SystemManager::SystemRunner::SystemRunner(SystemManager &sysMan) :
  threads(std::thread::hardware_concurrency()),
  exit(false) {
  for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
    threads.emplace_back(std::bind(&SystemRunner::threadProc, this, std::ref(sysMan)));
    Util::SetThreadName(threads.back(), ("SystemRunner thread " + std::to_string(i)).c_str());
  }
}

void SystemManager::SystemRunner::threadProc(SystemManager &sysMan) {
  while (!exit) {
    SystemTypeId processStid;
    { std::unique_lock<std::mutex> lk(queueMutex);
      queueCondVar.wait(lk, [this](){ return exit || queue.size() > 0; });
      if (exit) {
        return;
      }
      processStid = queue.front();
      queue.pop();
    }
    sysMan.systems[processStid]->update(dtime);
    { std::unique_lock<std::mutex> lk(queueMutex);
      SystemGraphNode &sgn = *sysMan.systemGraph[processStid];
      for (SystemTypeId nextStid : sgn.successors) {
        SystemGraphNode &nextSgn = *sysMan.systemGraph[nextStid];
        std::unique_lock<std::mutex> clk(nextSgn.counterMut);
        ++nextSgn.counter;
        if (nextSgn.counter == nextSgn.predecessors.size()) {
          queue.push(nextStid);
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

SystemManager::SystemRunner::~SystemRunner() {
  exit = true;
  queueCondVar.notify_all();
  for (std::thread &thr : threads) {
    if (thr.joinable()) {
      thr.join();
    }
  }
}

SystemManager::SystemGraphNode::SystemGraphNode(SystemTypeId system, SystemPtrVector &systems,
  SystemGraphNodeVector &graph) :
  system(system),
  systems(systems),
  graph(graph),
  index(indexUndef),
  lowlink(lowlinkUndef),
  onStack(false) {
}

using SystemGraphNode = SystemManager::SystemGraphNode;
using SystemGraphNodeVector = SystemManager::SystemGraphNodeVector;

static void strongconnect(SystemTypeId &index, std::stack<SystemTypeId> &S,
  SystemTypeId stid, SystemGraphNode &si, SystemGraphNodeVector &sinfo,
  SystemManager::SystemLoopVector &stronglyConnected) {
  // Set the depth index for v to the smallest unused index
  si.index = index;
  si.lowlink = index;
  ++index;
  S.push(stid);
  si.onStack = true;

  // Consider successors of s
  for (SystemTypeId wtid : si.successors) {
    SystemGraphNode &wi = *sinfo.at(wtid);
    if (wi.index == SystemGraphNode::indexUndef) {
      // Successor w has not yet been visited; recurse on it
      strongconnect(index, S, wtid, wi, sinfo, stronglyConnected);
      si.lowlink = std::min(si.lowlink, wi.lowlink);
    } else if (wi.onStack) {
      // Successor w is in stack S and hence in the current SCC
      si.lowlink = std::min(si.lowlink, wi.index);
    }
  }

  // If v is a root node, pop the stack and generate an SCC
  if (si.lowlink == si.index) {
    stronglyConnected.emplace_back();
    std::set<SystemTypeId> &strong = stronglyConnected.back();
    SystemTypeId wtid;
    do {
      wtid = S.top();
      S.pop();
      SystemGraphNode &wi = *sinfo.at(wtid);
      wi.onStack = false;
      strong.insert(wtid);
    } while (wtid != stid);
  }
}

static bool isReachableBySuccessors(const SystemGraphNode &start, SystemTypeId targetstid,
  const SystemGraphNodeVector &sinfo, std::stack<SystemTypeId> &path) {
  auto search = start.successors.find(targetstid);
  if (search != start.successors.end()) {
    path.push(targetstid);
    return true;
  } else {
    for (SystemTypeId stid : start.successors) {
      if (isReachableBySuccessors(*sinfo[stid], targetstid, sinfo, path)) {
        path.push(stid);
        return true;
      }
    }
  }
  return false;
}

static void df(SystemGraphNode &vertex0, SystemGraphNode &child0,
        std::set<SystemTypeId> &done, SystemGraphNodeVector &sinfo) {
  if (done.find(child0.system) != done.end()) {
    return;
  }
  for (SystemTypeId child : child0.successors) {
    vertex0.successors.erase(child);
    SystemGraphNode &childNi = *sinfo[child];
    childNi.predecessors.erase(vertex0.system);
    df(vertex0, childNi, done, sinfo);
  }
  done.insert(child0.system);
}

#if 0
static void dumpGraph(const std::string &path, const std::vector<std::unique_ptr<System>> &systems,
  const SystemGraphNodeVector &sinfo) {
  std::ofstream dot;
  dot.open(path, std::ios_base::out | std::ios_base::trunc);
  dot << "digraph SystemRunGraph {" << std::endl;
  for (const std::unique_ptr<SystemGraphNode> &sgnptr : sinfo) {
    if (sgnptr) {
      if (sgnptr->successors.size() > 0) {
        for (SystemTypeId succStid : sgnptr->successors) {
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

void SystemManager::computeSystemOrder() {
  systemGraph.clear();
  systemGraph.reserve(systems.size());
  // Each System can request to be run before other Systems, and after some others.
  // This allows for the injection of Systems between two other in the execution graph, which
  // otherwise wouldn't be possible with only 1 of the "run before" and "run after" condition.
  // 1-way dependencies exhibit a single issue in our case: creating loops in the graph. This is
  // fairly easy to detect thanks to Tarjan's strongly connected components algorithm.
  // Having Systems both requesting to run before and after some others, even more loops might get
  // introduced, i.e. A wants to run both before and after B, even indirectly; e.g. A runs before B
  // & after C, but C runs after B -- keeping in mind the graph we eventually want to get is a
  // plain Directed Acyclic Graph.
  //
  // A solution to successfully create an execution graph is the following:
  // Step 1: create basic "run before" directed graph.
  // Step 2: check for loops with Tarjan. If none, proceed.
  // Step 3: add "run after" deps by reversing them (B runs after A, so A runs before B).
  //         If there already exists a reverse path from B to A then it's incoherent
  //         (because transitively B runs after A, therefore cannot also run before A).
  // Step 4: delete any edges that can be transitively obtained (i.e. only keeping the longest
  //         paths), a.k.a. transitive reduction (as opposed to transitive closure).

  /* Step 0, O(V) */ {
    for (const std::unique_ptr<System> &sptr : systems) {
      if (sptr) {
        SystemTypeId stid = sptr->getTypeId();
        systemGraph.emplace_back(new SystemGraphNode(stid, systems, systemGraph));
      } else {
        systemGraph.emplace_back();
      }
    }
  }

  /* Step 1, O(V²) best & worst case (O(V(V-1)) really) */ {
    for (const std::unique_ptr<System> &sptr : systems) {
      if (sptr) {
        SystemTypeId stid = sptr->getTypeId();
        SystemGraphNode &si = *systemGraph[stid];
        for (const std::unique_ptr<System> &rbsptr : systems) {
          if (rbsptr and rbsptr != sptr and sptr->runsBefore(*rbsptr)) {
            SystemTypeId rbstid = rbsptr->getTypeId();
            si.successors.insert(rbstid);
            systemGraph[rbstid]->predecessors.insert(stid);
          }
        }
      }
    }
  }

  /* Step 2, O(|V| + |E|) worst case */ {
    SystemTypeId index = 0;
    std::stack<SystemTypeId> S;
    SystemLoopVector stronglyConnected;
    for (const std::unique_ptr<System> &sptr : systems) {
      if (sptr) {
        SystemTypeId stid = sptr->getTypeId();
        SystemGraphNode &si = *systemGraph[stid];
        if (si.index == SystemGraphNode::indexUndef) {
          strongconnect(index, S, stid, si, systemGraph, stronglyConnected);
        }
      }
    }
    for (const std::set<SystemTypeId> &elems : stronglyConnected) {
      if (elems.size() > 1) {
        throw RunsBeforeCreatesLoopsException(std::move(stronglyConnected));
      }
    }
  }

  /* Step 3, O(V² × <?>) */ {
    SystemLoopPath succReachPath;
    for (const std::unique_ptr<System> &sptr : systems) {
      if (sptr) {
        SystemTypeId stid = sptr->getTypeId();
        SystemGraphNode &si = *systemGraph[stid];
        for (const std::unique_ptr<System> &rasptr : systems) {
          if (rasptr and rasptr != sptr and sptr->runsAfter(*rasptr)) {
            SystemTypeId rastid = rasptr->getTypeId();
            // Check if the System to run after is already reachable, if yes, it would
            // create a loop; complain.
            if (isReachableBySuccessors(si, rastid, systemGraph, succReachPath)) {
              succReachPath.push(stid);
              throw RunsAfterCreatesLoopException(std::move(succReachPath));
            }
            systemGraph[rastid]->successors.insert(stid);
            si.predecessors.insert(rastid);
          }
        }
      }
    }
  }

  /* Step 4, O(<?>) */ {
    // http://stackoverflow.com/a/11237184/1616310
    for (std::unique_ptr<SystemGraphNode> &sgnptr : systemGraph) {
      if (sgnptr) {
        std::set<SystemTypeId> done;
        for (SystemTypeId child : sgnptr->successors) {
          df(*sgnptr, *systemGraph[child], done, systemGraph);
        }
      }
    }
  }
}

SystemManager::SystemManager(World &w) :
  w(w),
  systemRun(*this) {
}

void SystemManager::dispatchEvent(const Event &evt) {
  w.event.dispatch(evt);
}

void SystemManager::run(TDelta dtime) {
  std::unique_lock<std::mutex> lk(systemRun.runCountMutex);
  // Reset previous run count and set the dtime parameter to pass to Systems
  systemRun.runCount = 0;
  systemRun.dtime = dtime;

  // Count the Systems so we know when all of them were run. Also count the ones without
  // predecessors, i.e. the ones that runs first, to know how many threads to notify.
  SystemTypeId targetRunCount = 0;
  unsigned int startCount = 0;
  { std::unique_lock<std::mutex> qlk(systemRun.queueMutex);
    for (std::unique_ptr<SystemGraphNode> &sgn : systemGraph) {
      if (sgn) {
        sgn->counter = 0;
        if (sgn->predecessors.empty()) {
          systemRun.queue.push(sgn->system);
          ++startCount;
        }
        ++targetRunCount;
      }
    }
  }

  // Wake (notify) threads to start running Systems
  while (startCount--) {
    systemRun.queueCondVar.notify_one();
  }

  // Wait for all Systems to have run, i.e. runCount reached its target.
  systemRun.runCountCondVar.wait(lk, [this, targetRunCount]() {
    return systemRun.runCount == targetRunCount;
  });
}

} /* namespace radix */
