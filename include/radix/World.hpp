#ifndef WORLD_HPP
#define WORLD_HPP

#include <algorithm>
#include <array>
#include <random>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include <radix/Camera.hpp>
#include <radix/EntityManager.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/material/Material.hpp>
#include <radix/system/System.hpp>

namespace radix {

typedef std::pair<Entity*, Entity*> EntityPair;

class World {
protected:
  Entity *player;

public:
  struct SystemAddedEvent : public Event {
    /// @todo implement
  };
  struct SystemRemovedEvent : public Event {
    /// @todo implement
  };


  Camera camera;
  std::map<int, Material> materials;

  EventDispatcher event;

  EntityManager entities;
  std::map<std::string, std::vector<EntityPair>> entityPairs;

  /**
   * Gets the reference to the entity with specified ID.
   * @throws std::out_of_range if no entity with this ID is found.
   */
  inline Entity& getEntityById(EntityId id) {
    return entities.getById(id);
  }

  /**
   * Gets the reference to the entity with specified name.
   * @throws std::out_of_range if no entity with this name is found.
   * @note No name unicity is enforced by the engine itself. The first entity in the entity list
   * matching this name will be returned.
   */
  inline Entity& getEntityByName(const std::string &name) {
    return entities.getByName(name);
  }


  /// @todo make publicly read-only
  std::vector<std::unique_ptr<System>> systems;
  std::map<SystemTypeId, System*> systemsById;
  template<class T, typename... ArgsT> void addSystem(ArgsT... args) {
    std::unique_ptr<System> newSys(new T(*this, std::forward<ArgsT>(args)...));
    systemsById.emplace(std::piecewise_construct, std::forward_as_tuple(System::getTypeId<T>()),
      std::forward_as_tuple(&*systems.emplace(std::upper_bound(systems.begin(), systems.end(), newSys,
        [](const std::unique_ptr<System> &a, const std::unique_ptr<System> &b) -> bool {
          return a->runsBefore(*b);
        }), std::move(newSys))));
  }
  template<class T> T& getSystem() {
    return (T&)*systemsById.at(System::getTypeId<T>());
  }
  /// @todo Implement template<class T> void removeSystem()


  World();
  ~World();
  void create();
  void destroy();

  inline double getTime() const {
    return gameTime;
  }
  void update(double dtime);

  Entity& getPlayer();

private:
  double gameTime;
  uint32_t lastUpdateTime;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
