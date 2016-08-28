#ifndef RADIX_PHYSICS_SYSTEM_HPP
#define RADIX_PHYSICS_SYSTEM_HPP

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/system/System.hpp>

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btGhostPairCallback;

namespace radix {

class CollisionDispatcher;
class Uncollider;

class PhysicsSystem : public System {
private:
  EventDispatcher::CallbackHolder cbCompAdd, cbCompRem;

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

  TypeId getTypeId() const {
    return System::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  btDiscreteDynamicsWorld& getPhysWorld() const {
    return *physWorld;
  }

  void update(TDelta timeDelta);
};

} /* namespace radix */

#endif /* RADIX_PHYSICS_SYSTEM_HPP */
