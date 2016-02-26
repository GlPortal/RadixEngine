#include <radix/EntityManager.hpp>

#include <stdexcept>

#include <radix/World.hpp>

namespace radix {

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
  for (auto it = begin(); it != end(); ++it) {
    if (it->name == name) {
      return *it;
    }
  }
  throw std::out_of_range("No entity found by that name");
}

} /* namespace radix */
