#include <radix/World.hpp>

#include <radix/data/map/MapLoader.hpp>
#include <radix/data/map/MapListLoader.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/env/Environment.hpp>
#include <radix/component/Health.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/SoundSource.hpp>
#include <radix/component/SoundListener.hpp>
#include <radix/component/LightSource.hpp>
#include <radix/component/Player.hpp>

namespace radix {

World::World(InputSource &input) :
  input(input),
  systems(*this),
  entityManager(*this),
  gameTime(0),
  lastUpdateTime(0){
  input.addDispatcher(event);
  player = &entityManager.create();
  player->addComponent<Transform>().setPosition(Vector3f(2.5, 1, 5));
  player->addComponent<Health>();
  player->addComponent<SoundSource>();
  player->addComponent<SoundListener>();
  player->addComponent<Player>();
  player->getComponent<Player>().addTask<PlayerMoveTask>();
  player->getComponent<Player>().addTask<PlayerTriggerTask>();
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

void World::create() {
  lastUpdateTime = SDL_GetTicks();
}

void World::destroy() {
}

void World::update(TDelta dtime) {
  gameTime += dtime;
  systems.run(dtime);
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
