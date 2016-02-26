#include <radix/World.hpp>

#include <climits>
#include <cmath>
#include <vector>
#include <cstdio>

#include <SDL2/SDL_timer.h>

// TODO remove
#include <radix/renderer/GWENRenderer.hpp>

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

#include <radix/core/math/Math.hpp>
#include <radix/core/math/Vector2f.hpp>
#include <radix/core/math/Vector3f.hpp>

#include <radix/input/Input.hpp>

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
