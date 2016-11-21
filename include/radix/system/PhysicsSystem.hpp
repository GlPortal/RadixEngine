#ifndef RADIX_PHYSICS_SYSTEM_HPP
#define RADIX_PHYSICS_SYSTEM_HPP

#include <unordered_set>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/system/System.hpp>

namespace radix {

class CollisionDispatcher;
class Uncollider;
class BaseGame;

struct CollisionInfo {
  btCollisionObject *body0;
  btCollisionObject *body1;
  CollisionInfo(btCollisionObject* body0, btCollisionObject* body1)
    : body0(body0), body1(body1) {}
};

class CollisionInfoHash {
public:
  unsigned long operator()(const CollisionInfo& info) const {
    return (size_t) info.body0 + (size_t) info.body1;
  }
};

class CollisionInfoEqual {
public:
  bool operator()(const CollisionInfo& info0, const CollisionInfo& info1) const {
    return (info0.body0 == info1.body0) && (info0.body1 == info1.body1) &&
      (info0.body0->getUserPointer() == info1.body0->getUserPointer()) &&
      (info0.body1->getUserPointer() == info1.body1->getUserPointer());
  }
};

class PhysicsSystem : public System {
private:
  EventDispatcher::CallbackHolder cbCompAdd, cbCompRem;

  friend class Uncollider;
  Uncollider *filterCallback;
  BaseGame* game;

  btBroadphaseInterface *broadphase;
  btDefaultCollisionConfiguration *collisionConfiguration;
  CollisionDispatcher *dispatcher;
  btSequentialImpulseConstraintSolver *solver;
  btDiscreteDynamicsWorld *physicsWorld;
  btGhostPairCallback *gpCallback;
public:
  PhysicsSystem(World&, BaseGame* game);
  ~PhysicsSystem();

  const char* getName() const {
    return "PhysicsSystem";
  }

  TypeId getTypeId() const {
    return System::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  btDiscreteDynamicsWorld& getPhysicsWorld() const {
    return *physicsWorld;
  }

  void update(TDelta timeDelta);

  static std::unordered_set<CollisionInfo, CollisionInfoHash,
    CollisionInfoEqual> collisions;

  static bool contactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1);

  static bool contactDestroyedCallback(void* userPersistentData);
};

} /* namespace radix */

#endif /* RADIX_PHYSICS_SYSTEM_HPP */
