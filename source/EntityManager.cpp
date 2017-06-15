#include <radix/EntityManager.hpp>

#include <radix/World.hpp>

namespace radix {

void EntityManager::entityAdded(Entity &ent) {
  idMap.emplace(std::piecewise_construct,
    std::forward_as_tuple(ent.id),
    std::forward_as_tuple(ent));
  ent.init();
  world.event.dispatch(EntityCreatedEvent(ent));
}

void EntityManager::changeEntityName(Entity &ent, const std::string &from, const std::string &to) {
  if (!from.empty()) {
    nameMap.erase(from);
  }
  if (!to.empty()) {
    nameMap.emplace(std::piecewise_construct,
      std::forward_as_tuple(to),
      std::forward_as_tuple(ent));
  }
}

EntityManager::EntityManager(World &w) :
  m_lastAllocatedId(1),
  world(w) {
}

Entity& EntityManager::getById(EntityId id) {
  for (Entity &ent : *this) {
    if (ent.id == id) {
      return ent;
    }
  }
  throw std::out_of_range("No entity found by that ID");
}

Entity& EntityManager::getByName(const std::string &name) {
  auto it = nameMap.find(name);
  if (it == nameMap.end()) {
    throw std::out_of_range("No entity found by that name");
  }
  return it->second;
}

} /* namespace radix */
