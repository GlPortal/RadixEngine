#include <radix/World.hpp>

#include <climits>
#include <cstdio>
#include <cmath>
#include <limits>

#include <SDL2/SDL_timer.h>

#include <radix/map/MapLoader.hpp>
#include <radix/map/MapListLoader.hpp>

#include <radix/renderer/Renderer.hpp>
#include <radix/env/Environment.hpp>
#include <radix/Camera.hpp>

#include <radix/component/Health.hpp>
#include <radix/component/Transform.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/SoundSource.hpp>
#include <radix/component/SoundListener.hpp>
#include <radix/component/LightSource.hpp>
#include <radix/component/Player.hpp>

#include <radix/core/math/Vector3f.hpp>

namespace radix {

World::SystemRunner::SystemRunner(World &w) :
  threads(std::thread::hardware_concurrency()),
  exit(false),
  dtime(0) {
  for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
    threads.emplace_back(std::bind(&SystemRunner::threadProc, this, std::ref(w)));
    Util::SetThreadName(threads.back(), ("SystemRunner thread " + std::to_string(i)).c_str());
  }
}

void World::SystemRunner::threadProc(World &w) {
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
    w.systems[processStid]->update(dtime);
    { std::unique_lock<std::mutex> lk(queueMutex);
      SystemGraphNode &sgn = *w.systemGraph[processStid];
      for (SystemTypeId nextStid : sgn.successors) {
        SystemGraphNode &nextSgn = *w.systemGraph[nextStid];
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

World::SystemRunner::~SystemRunner() {
  exit = true;
  queueCondVar.notify_all();
  for (std::thread &thr : threads) {
    if (thr.joinable()) {
      thr.join();
    }
  }
}

World::SystemGraphNode::SystemGraphNode(SystemTypeId system, SystemPtrVector &systems,
  SystemGraphNodeVector &graph) :
  system(system),
  systems(systems),
  graph(graph),
  index(indexUndef),
  lowlink(lowlinkUndef),
  onStack(false) {
}

World::World(InputSource &input) :
  input(input),
  entities(*this),
  systemRun(*this),
  gameTime(0),
  lastUpdateTime(0){
  input.addDispatcher(event);
  player = &entities.create();
  player->addComponent<Transform>().setPosition(Vector3f(2.5, 1, 5));
  player->addComponent<Health>();
  player->addComponent<SoundSource>();
  player->addComponent<SoundListener>();
  player->addComponent<Player>();
}

World::~World() {
  input.removeDispatcher(event);
}

void World::setConfig(radix::Config &config){
  this->config = config;
}

radix::Config &World::getConfig(){
  return this->config;
}

using SystemGraphNode = World::SystemGraphNode;
using SystemGraphNodeVector = World::SystemGraphNodeVector;

static void strongconnect(SystemTypeId &index, std::stack<SystemTypeId> &S,
  SystemTypeId stid, SystemGraphNode &si, SystemGraphNodeVector &sinfo,
  World::SystemLoopVector &stronglyConnected) {
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

void World::computeSystemOrder() {
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

void World::create() {
  lastUpdateTime = SDL_GetTicks();
}

void World::destroy() {
}

void World::update(TDelta dtime) {
  gameTime += dtime;
  { std::unique_lock<std::mutex> lk(systemRun.runCountMutex);
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
#if 0
  /// @todo
  Entity &player = *this->player;
  Health &plrHealth = player.getComponent<Health>();
  Transform &plrTform = player.getComponent<Transform>();
  Player &plrComp = player.getComponent<Player>();

  if (not plrHealth.isAlive()) {
    plrTform.setPosition(scene->start->getComponent<Transform>().getPosition());
    plrHealth.revive();
    hidePortals();
  }

  for (Entity &e : scene->entities) {
    // Trigger
    if (e.hasComponent<Trigger>()) {
      const Trigger &trigger = e.getComponent<Trigger>();
      const Transform &trTform = e.getComponent<Transform>();
      BoxCollider playerCollider(plrTform.position, plrTform.scale);
      BoxCollider triggerCollider(trTform.position, trTform.scale);

      if (playerCollider.collidesWith(triggerCollider)) {
        if (trigger.type == "radiation") {
          player.getComponent<Health>().harm(.1f);
          System::Log(Verbose) << "Radiation touched.";
        } else if (trigger.type == "death") {
          player.getComponent<Health>().kill();
          System::Log(Verbose) << "Death touched.";
        } else if (trigger.type == "win") {
          if (currentLevel + 1 < mapList.size()) {
            currentLevel++;
            loadScene(mapList[currentLevel]);
          } else {
            scene->screen->enabled = true;
          }
          System::Log(Verbose) << "Win touched.";
        } else if (trigger.type == "map") {
          System::Log(Verbose) << "Map trigger touched.";
          throw __FILE__ ": Map trigger type de-implemented, please reimplement";
          //loadScene(trigger.reference);
        } else if (trigger.type == "button") {
          System::Log(Verbose) << "Button touched.";
          printf("Button touched\n");
        } else {
          System::Log(Verbose) << "Other trigger touched. " << trigger.type;
        }
      }
    }
  }*/

  //Check if the end of the level has been reached
  float distToEnd = (scene->end->getComponent<Transform>().getPosition() -
                     plrTform.getPosition()).length();
  if (distToEnd < 1) {
    if (currentLevel + 1 < mapList.size()) {
      currentLevel++;
      loadScene(mapList[currentLevel]);
    } else {
      scene->screen->enabled = true;
      scene->screen->title   = "Game Over";
      scene->screen->text    = "Hit q to quit the game.";
    }
  }
#endif
}

#if 0

void World::render(double dtime) {
  renderer->render(dtime, scene->camera);
  if (isPhysicsDebugEnabled) {
    scene->physics.world->debugDrawWorld();
    pdd.render(scene->camera);
  }
  window->gwenCanvas->RenderCanvas();
  window->gwenRenderer->End();
}
#endif

Entity& World::getPlayer() {
  return *player;
}

} /* namespace radix */
