#include <radix/World.hpp>

#include <climits>
#include <cstdio>
#include <cmath>
#include <limits>
#include <vector>

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

#include <iostream>
#include <fstream>

namespace radix {

World::World() :
  entities(*this),
  gameTime(0),
  lastUpdateTime(0) {
  player = &entities.create();
  player->addComponent<Transform>().setPosition(Vector3f(2.5, 1, 5));
  player->addComponent<Health>();
  player->addComponent<SoundSource>();
  player->addComponent<SoundListener>();
  player->addComponent<Player>();
}

World::~World() = default;

struct SystemGraphNodeInfo {
  SystemTypeId system, index, lowlink;
  bool sysExists, onStack;
  std::set<SystemTypeId> predecessors, successors;
  static constexpr decltype(index) indexUndef = std::numeric_limits<decltype(index)>::max();
  static constexpr decltype(lowlink) lowlinkUndef = std::numeric_limits<decltype(lowlink)>::max();
  SystemGraphNodeInfo() :
    index(indexUndef),
    lowlink(lowlinkUndef),
    sysExists(false),
    onStack(false) {
  }
};

static void strongconnect(SystemTypeId &index, std::stack<SystemTypeId> &S,
  SystemTypeId stid, SystemGraphNodeInfo &si,
  std::vector<SystemGraphNodeInfo> &sinfo,
  World::SystemLoopVector &stronglyConnected) {
  // Set the depth index for v to the smallest unused index
  si.index = index;
  si.lowlink = index;
  ++index;
  S.push(stid);
  si.onStack = true;

  // Consider successors of s
  for (SystemTypeId wtid : si.successors) {
    SystemGraphNodeInfo &wi = sinfo.at(wtid);
    if (wi.index == SystemGraphNodeInfo::indexUndef) {
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
      SystemGraphNodeInfo &wi = sinfo.at(wtid);
      wi.onStack = false;
      strong.insert(wtid);
    } while (wtid != stid);
  }
}

static bool isReachableBySuccessors(const SystemGraphNodeInfo &start, SystemTypeId targetstid,
  const std::vector<SystemGraphNodeInfo> &sinfo, std::stack<SystemTypeId> &path) {
  auto search = start.successors.find(targetstid);
  if (search != start.successors.end()) {
    path.push(targetstid);  
    return true;
  } else {
    for (SystemTypeId stid : start.successors) {
      if (isReachableBySuccessors(sinfo[stid], targetstid, sinfo, path)) {
        path.push(stid);
        return true;
      }
    }
  }
  return false;
}

static void df(SystemGraphNodeInfo &vertex0, SystemGraphNodeInfo &child0,
        std::set<SystemTypeId> &done, std::vector<SystemGraphNodeInfo> &sinfo) {
  if (done.find(child0.system) != done.end()) {
    return;
  }
  for (SystemTypeId child : child0.successors) {
    vertex0.successors.erase(child);
    SystemGraphNodeInfo &childNi = sinfo[child];
    childNi.predecessors.erase(vertex0.system);
    df(vertex0, childNi, done, sinfo);
  }
  done.insert(child0.system);
}

static void dumpGraph(const std::string &path, const std::vector<std::unique_ptr<System>> &systems,
  const std::vector<SystemGraphNodeInfo> &sinfo) {
  std::ofstream dot;
  dot.open(path, std::ios_base::out | std::ios_base::trunc);
  dot << "digraph SystemRunGraph {" << std::endl;
  for (const SystemGraphNodeInfo &sgni : sinfo) {
    if (sgni.sysExists) {
      if (sgni.successors.size() > 0) {
        for (SystemTypeId succStid : sgni.successors) {
          dot << systems[sgni.system]->getName() << " -> " << systems[succStid]->getName() << ';' << std::endl;
        }
      } else {
        dot << systems[sgni.system]->getName() << ';' << std::endl;
      }
    }
  }
  dot << "}";
  dot.close();
  std::system(("dot -O -Tpng " + path).c_str());
}

void World::computeSystemOrder() {
  std::vector<SystemGraphNodeInfo> sinfo(systems.size());
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

  /* Step 1, O(V²) best & worst case (O(V(V-1)) really) */ {
    for (const std::unique_ptr<System> &sptr : systems) {
      if (sptr) {
        SystemTypeId stid = sptr->getTypeId();
        SystemGraphNodeInfo &si = sinfo[stid];
        si.sysExists = true;
        si.system = stid;
        for (const std::unique_ptr<System> &rbsptr : systems) {
          if (rbsptr and rbsptr != sptr and sptr->runsBefore(*rbsptr)) {
            SystemTypeId rbstid = rbsptr->getTypeId();
            si.successors.insert(rbstid);
            sinfo[rbstid].predecessors.insert(stid);
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
        SystemGraphNodeInfo &si = sinfo[stid];
        if (si.index == SystemGraphNodeInfo::indexUndef) {
          strongconnect(index, S, stid, si, sinfo, stronglyConnected);
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
        SystemGraphNodeInfo &si = sinfo[stid];
        for (const std::unique_ptr<System> &rasptr : systems) {
          if (rasptr and rasptr != sptr and sptr->runsAfter(*rasptr)) {
            SystemTypeId rastid = rasptr->getTypeId();
            // Check if the System to run after is already reachable, if yes, it would
            // create a loop; complain.
            if (isReachableBySuccessors(si, rastid, sinfo, succReachPath)) {
              succReachPath.push(stid);
              throw RunsAfterCreatesLoopException(std::move(succReachPath));
            }
            sinfo[rastid].successors.insert(stid);
            si.predecessors.insert(rastid);
          }
        }
      }
    }
  }

  /* Step 4, O(<?>) */ {
    // http://stackoverflow.com/a/11237184/1616310
    for (SystemGraphNodeInfo &sgni : sinfo) {
      if (sgni.sysExists) {
        std::set<SystemTypeId> done;
        for (SystemTypeId child : sgni.successors) {
          df(sgni, sinfo[child], done, sinfo);
        }
      }
    }
  }

  //dumpGraph("/tmp/radixSystemGraph", systems, sinfo);
}

void World::create() {
  lastUpdateTime = SDL_GetTicks();
}

void World::destroy() {
}

#if 0
/// @todo recreate scene loading utility
void World::loadScene(const std::string &path) {
  // Delete previous scene
  delete scene;
  currentScenePath = path;
  scene = MapLoader::getScene(path);

  if (justStarted) {
    Entity &player = *scene->player;
    Player &plr = player.getComponent<Player>();
    plr.frozen = true;
    scene->screen->enabled = true;
    justStarted = false;
  }

  scene->world = this;
  plrSys.setScene(scene);
  phys.setScene(scene);
  scene->physics.world->setDebugDrawer(&pdd);
  scene->physics.setGravity(0, -9.8, 0);
  prtl.setScene(scene);
  renderer->setScene(scene);

  Environment::dispatcher.dispatch(Event::loadScene);
}
#endif

void World::update(double dtime) {
  gameTime += dtime;
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

  plrSys.update(dtime);
  phys.update(dtime);
  prtl.update(dtime);

/*
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

  // Parent camera to player
  camera.setPerspective();
  int vpWidth, vpHeight;
  renderer->getViewport()->getSize(&vpWidth, &vpHeight);
  camera.setAspect((float)vpWidth / vpHeight);
  camera.setPosition(plrTform.getPosition() + Vector3f(0, plrTform.getScale().y/2, 0));
  camera.setOrientation(plrComp.getHeadOrientation());

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
void World::shootPortal(int button) {
  WorldHelper::shootPortal(button, scene);
}

Renderer* World::getRenderer() const {
  return renderer;
}

// TODO: move elsewhere. World is world, not renderer.
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

} /* namespace glPortal */
