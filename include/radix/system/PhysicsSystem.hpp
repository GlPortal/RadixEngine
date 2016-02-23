#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <radix/scene/Scene.hpp>

namespace radix {

class Uncollider;

class PhysicsSystem {
private:
  Scene *scene;
  Uncollider *filterCallback;

public:
  PhysicsSystem();
  ~PhysicsSystem();
  void setScene(Scene *scene);
  void update(float dtime);
};

} /* namespace radix */

#endif /* PHYSICS_SYSTEM_HPP */
