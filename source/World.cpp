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
  gameTime(0),
  lastUpdateTime(0),
  input(input),
  systems(*this),
  entityManager(*this){
  input.addDispatcher(event);
  initPlayer();
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
}

Entity& World::getPlayer() {
  return *player;
}

void World::initPlayer() {
  player = &entityManager.create();
  player->addComponent<Transform>().setPosition(Vector3f(2.5, 1, 5));
  player->addComponent<Health>();
  player->addComponent<SoundSource>();
  player->addComponent<SoundListener>();
  player->addComponent<Player>();
  player->getComponent<Player>().addTask<PlayerMoveTask>();
  player->getComponent<Player>().addTask<PlayerTriggerTask>();
}

} /* namespace radix */
