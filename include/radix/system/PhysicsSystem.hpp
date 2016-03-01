#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/physics/CollisionDispatcher.hpp>
#include <radix/system/System.hpp>

namespace radix {

class Uncollider;

class PhysicsSystem : public System {
private:
  EventDispatcher::CallbackPointer cpCompAdd, cpCompRem;

  friend class Uncollider;
  Uncollider *filterCallback;

  btBroadphaseInterface *broadphase;
  btDefaultCollisionConfiguration *collisionConfiguration;
  CollisionDispatcher *dispatcher;
  btSequentialImpulseConstraintSolver *solver;
  btDiscreteDynamicsWorld *physWorld;
  btGhostPairCallback *gpCallback;

public:
  PhysicsSystem(World&);
  ~PhysicsSystem();

  const char* getName() const {
    return "PhysicsSystem";
  }
  void update(float dtime);
};

} /* namespace radix */

#endif /* PHYSICS_SYSTEM_HPP */
