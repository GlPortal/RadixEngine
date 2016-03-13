#include <radix/EntityManager.hpp>

#include <stdexcept>

#include <radix/World.hpp>

namespace radix {

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
  world(w) {
}

Entity& EntityManager::create() {
  const EntityId id = Util::Rand();
  emplace_back(*this, id);
  return back();
}

Entity& EntityManager::getById(EntityId id) {
  for (auto it = begin(); it != end(); ++it) {
    if (it->id == id) {
      return *it;
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
