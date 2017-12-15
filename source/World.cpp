#include <radix/World.hpp>

#include <radix/data/map/MapLoader.hpp>
#include <radix/data/map/MapListLoader.hpp>
#include <radix/entities/Player.hpp>
#include <radix/env/Environment.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/BaseGame.hpp>

namespace radix {

World::World(BaseGame &game) :
  gameTime(0),
  lastUpdateTime(0),
  game(game),
  simulations(*this),
  entityManager(*this) {
  event.debugLogLevel = EventDispatcher::DebugLogLevel::DispatchedEventsRepr;
}

World::~World() {
}

void World::setConfig(radix::Config &config) {
  this->config = config;
}

radix::Config& World::getConfig() {
  return this->config;
}

void World::onCreate() {
  lastUpdateTime = SDL_GetTicks();
  camera = std::make_unique<Camera>();
}

void World::onStart() {
  input = &game.getWindow();
  input->addDispatcher(event);
}

void World::update(TDelta dtime) {
  gameTime += dtime;
  simulations.run(dtime);
  for (Entity &ent : entityManager) {
    ent.tick(dtime);
  }
  entityManager.doMaintenance();
}

void World::onStop() {
  input->removeDispatcher(event);
  input = nullptr;
}

void World::onDestroy() {
}

entities::Player& World::getPlayer() {
  return *player;
}

void World::initPlayer() {
  player = &entityManager.create<entities::Player>();
}

} /* namespace radix */
