#include <radix/EntityManager.hpp>

#include <radix/scene/Scene.hpp>

namespace radix {

EntityManager::EntityManager(Scene &scene) :
  scene(scene) {
}

Entity& EntityManager::create() {
  emplace_back(*this);
  return back();
}

} /* namespace radix */
