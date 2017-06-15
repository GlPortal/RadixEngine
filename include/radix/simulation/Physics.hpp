#ifndef RADIX_SIMULATION_PHYSICS_HPP
#define RADIX_SIMULATION_PHYSICS_HPP

#include <unordered_set>

#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/simulation/Simulation.hpp>

namespace radix {

class CollisionDispatcher;
class Uncollider;

// TODO: find a better place to put that
class ContactPlayerCallback : public btCollisionWorld::ContactResultCallback {
public:
  ContactPlayerCallback(BaseGame &game) : btCollisionWorld::ContactResultCallback(), game(game) {}
  BaseGame &game;

  virtual btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,
                                   int partId0, int index0,const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
    entities::Player *player = dynamic_cast<entities::Player*>(
        reinterpret_cast<Entity*>(colObj0Wrap->getCollisionObject()->getUserPointer()));
    entities::Trigger *trigger = dynamic_cast<entities::Trigger*>(
        reinterpret_cast<Entity*>(colObj1Wrap->getCollisionObject()->getUserPointer()));

    if (trigger && player) {
      trigger->onUpdate(game);
      player->trigger = trigger;
    }
    return 0;
  }
};

namespace simulation {

class CheckCollisionCallback : public btCollisionWorld::ContactResultCallback {
public:
  bool *remove;
  CheckCollisionCallback(bool* remove) : ContactResultCallback(), remove(remove) { }

  virtual btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,
      int partId0, int index0,const btCollisionObjectWrapper* colObj1Wrap, int partId1,
      int index1) {
    *remove = false;
    return 0;
  }
};

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

class Physics final : public Simulation {
private:
  EventDispatcher::CallbackHolder cbEntityAdd, cbEntityRem;

  friend class Uncollider;
  Uncollider *filterCallback;
  BaseGame *game;

  btBroadphaseInterface *broadphase;
  btDefaultCollisionConfiguration *collisionConfiguration;
  CollisionDispatcher *dispatcher;
  btSequentialImpulseConstraintSolver *solver;
  btDiscreteDynamicsWorld *physicsWorld;
  btGhostPairCallback *gpCallback;

public:
  Physics(World&, BaseGame *game);
  ~Physics();

  const char* getName() const {
    return "Physics";
  }

  btDiscreteDynamicsWorld& getPhysicsWorld() const {
    return *physicsWorld;
  }

  void update(TDelta timeDelta);

  static std::unordered_set<CollisionInfo, CollisionInfoHash,
    CollisionInfoEqual> collisions;

  static bool contactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1);

  void checkCollisions();

  struct CollisionAddedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/simulation/Physics:CollisionAdded";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    CollisionInfo &info;
    BaseGame &game;
    CollisionAddedEvent(CollisionInfo &info, BaseGame &game) : info(info), game(game) { }
  };

  struct CollisionRemovedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/simulation/Physics:CollisionRemoved";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    CollisionInfo &info;
    BaseGame &game;
    CollisionRemovedEvent(CollisionInfo &info, BaseGame &game) : info(info), game(game) { }
  };
};

} /* namespace simulation */
} /* namespace radix */

#endif /* RADIX_SIMULATION_PHYSICS_HPP */
