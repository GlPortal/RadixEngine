#ifndef RADIX_WORLD_HPP
#define RADIX_WORLD_HPP

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include <radix/Camera.hpp>
#include <radix/EntityManager.hpp>
#include <radix/core/types/TimeDelta.hpp>
#include <radix/core/state/HandleGameFunction.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/data/material/Material.hpp>
#include <radix/SimulationManager.hpp>
#include <radix/data/map/Destination.hpp>
#include <radix/input/InputSource.hpp>
#include <radix/env/Config.hpp>
#include <radix/entities/Player.hpp>

namespace radix {

typedef std::pair<Entity*, Entity*> EntityPair;

class World {
private:
  radix::Config config;
  double gameTime;
  uint32_t lastUpdateTime;
  void initPlayer();
protected:
  entities::Player *player;
public:
  InputSource &input;
  World(InputSource &input);
  void setConfig(radix::Config &config);
  radix::Config &getConfig();
  ~World();
  void create();
  void destroy();

  inline double getTime() const {
    return gameTime;
  }

  void update(TDelta dtime);
  entities::Player& getPlayer();
  std::map<int, Material> materials;
  EventDispatcher event;
  SimulationManager simulations;
  EntityManager entityManager;
  std::unique_ptr<Camera> camera;

  // Convenience
  std::map<std::string, Entity*> entityAliases;
  std::map<std::string, std::vector<Entity*>> entityGroups;
  std::map<std::string, std::vector<EntityPair>> entityPairs;
  std::map<std::string, Destination> destinations;

  // Game States
  std::stack<HandleGameFunction> stateFunctionStack;

  /**
   * Gets the reference to the entity with specified ID.
   * @throws std::out_of_range if no entity with this ID is found.
   */
  inline Entity& getEntityById(EntityId id) {
    return entityManager.getById(id);
  }

  /**
   * Gets the reference to the entity with specified name.
   * @throws std::out_of_range if no entity with this name is found.
   */
  inline Entity& getEntityByName(const std::string &name) {
    return entityManager.getByName(name);
  }
};

} /* namespace glPortal */

#endif /* RADIX_WORLD_HPP */
