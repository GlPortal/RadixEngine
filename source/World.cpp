#include <radix/World.hpp>

#include <radix/data/map/MapLoader.hpp>
#include <radix/data/map/MapListLoader.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/env/Environment.hpp>

namespace radix {

World::World(BaseGame &game, InputSource &input) :
  gameTime(0),
  lastUpdateTime(0),
  game(game),
  input(input),
  simulations(*this),
  entityManager(*this){
  input.addDispatcher(event);
}

World::~World() {
  input.removeDispatcher(event);
}

void World::setConfig(radix::Config &config) {
  this->config = config;
}

radix::Config& World::getConfig() {
  return this->config;
}

void World::create() {
  lastUpdateTime = SDL_GetTicks();
  camera = std::make_unique<Camera>();
  std::vector<EntityPair> portalPairs;
  entityPairs.insert(std::pair<std::string, std::vector<EntityPair>>("portalPairs", portalPairs));
}

void World::destroy() {
}

void World::update(TDelta dtime) {
  gameTime += dtime;
  simulations.run(dtime);
  for (Entity &ent : entityManager) {
    ent.tick(dtime);
  }
}

entities::Player& World::getPlayer() {
  return *player;
}

void World::initPlayer() {
  player = &entityManager.create<entities::Player>();
  player->setPosition(Vector3f(2.5, 1, 5));
}

} /* namespace radix */
