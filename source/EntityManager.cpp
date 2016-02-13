#include <radix/EntityManager.hpp>

#include <radix/scene/Scene.hpp>

namespace glPortal {

EntityManager::EntityManager(Scene &scene) :
  scene(scene) {
}

Entity& EntityManager::create() {
  emplace_back(*this);
  return back();
}

} /* namespace glPortal */
