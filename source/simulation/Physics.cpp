#include <radix/simulation/Physics.hpp>

#include <typeinfo>

#include <radix/entities/Player.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/entities/traits/RigidBodyTrait.hpp>
#include <radix/physics/CollisionDispatcher.hpp>
#include <radix/physics/Uncollider.hpp>
#include <radix/World.hpp>

using namespace radix::entities;

static const std::string Tag = "sim::Physics";

namespace radix {
namespace simulation {

std::unordered_set<CollisionInfo, CollisionInfoHash, CollisionInfoEqual> Physics::collisions;
Physics *instance; //< TODO: get rid of this

Physics::Physics(World &world, BaseGame *game) :
  Simulation(world),
  filterCallback(nullptr),
  game(game),
  broadphase(new btDbvtBroadphase),
  collisionConfiguration(new btDefaultCollisionConfiguration()),
  dispatcher(new CollisionDispatcher(collisionConfiguration)),
  solver(new btSequentialImpulseConstraintSolver),
  physicsWorld(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
                                           collisionConfiguration)),
  gpCallback(new btGhostPairCallback) {
  broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(gpCallback);
  filterCallback = new Uncollider(world);
  //physWorld->getPairCache()->setOverlapFilterCallback(filterCallback);
  dispatcher->setNearCallback(Uncollider::nearCallback);
  physicsWorld->setGravity(btVector3(0, -9.80665, 0));

  gContactProcessedCallback = reinterpret_cast<ContactProcessedCallback>
      (&Physics::contactProcessedCallback);

  cbEntityAdd = world.event.addObserver(EntityManager::EntityCreatedEvent::Type,
                                      [this](const radix::Event &ebase) {
      const EntityManager::EntityCreatedEvent &e =
          static_cast<const EntityManager::EntityCreatedEvent&>(ebase);
      if (typeid(e.entity) == typeid(Trigger)) {
        Util::Log(Verbose, Tag) << "Adding Trigger(" << e.entity.id << ')';
        Trigger &trigger = dynamic_cast<Trigger&>(e.entity);
        trigger.getBulletGhostObject()->setCollisionFlags(
            trigger.getBulletGhostObject()->getCollisionFlags() |
            btCollisionObject::CF_NO_CONTACT_RESPONSE);
        physicsWorld->addCollisionObject(trigger.getBulletGhostObject());
      }
    });
  cbEntityRem = world.event.addObserver(EntityManager::EntityRemovedEvent::Type,
                                      [this](const radix::Event &ebase) {
      const EntityManager::EntityRemovedEvent &e =
          static_cast<const EntityManager::EntityRemovedEvent&>(ebase);
      RigidBodyTrait *rbtp = dynamic_cast<RigidBodyTrait*>(&e.entity);
      if (rbtp) {
        physicsWorld->removeRigidBody(rbtp->body);
      }
    });

  for (Entity &entity : world.entityManager) {
    if (dynamic_cast<RigidBodyTrait*>(&entity) != nullptr) {
      cbEntityAdd(EntityManager::EntityCreatedEvent(entity));
    }
    if (typeid(entity) == typeid(Player)) {
      cbEntityAdd(EntityManager::EntityCreatedEvent(entity));
    }
  }

  instance = this;
}

Physics::~Physics() {
  delete filterCallback;
}

void Physics::update(TDelta timeDelta) {
  filterCallback->beforePhysicsStep();
  physicsWorld->stepSimulation(timeDelta.sec(), 10);
  for (Entity &entity : world.entityManager) {
    RigidBodyTrait *rbtp = dynamic_cast<RigidBodyTrait*>(&entity);
    if (rbtp and rbtp->body and not rbtp->body->isStaticObject()) {
      btTransform btTform;
      rbtp->body->getMotionState()->getWorldTransform(btTform);
      entity.privSetPosition(btTform.getOrigin());
      entity.privSetOrientation(btTform.getRotation());
    }
  }

  ContactPlayerCallback callback;
  physicsWorld->contactTest(world.getPlayer().obj, callback);
  checkCollisions();
}

bool Physics::contactProcessedCallback(btManifoldPoint &cp, void *body0, void *body1) {
  CollisionInfo pair((btCollisionObject*) body0, (btCollisionObject*) body1);
  auto found = collisions.find(pair);
  if (found == collisions.end()) {
    found = collisions.insert(pair).first;
    World &world = reinterpret_cast<Entity*>(
            reinterpret_cast<btCollisionObject*>(body0)->getUserPointer()
        )->world;
    world.event.dispatch(CollisionAddedEvent(pair, world));
  }
  cp.m_userPersistentData = (void*) &*found;
  return true; /* the return value is ignored */
}

void Physics::checkCollisions() {
  if (!collisions.empty()) {
    std::vector<CollisionInfo*> toRemove;
    /*vector because we need to avoid modifying a data structure when we iterate over it */

    for (CollisionInfo info : collisions) {
      bool remove = true;
      CheckCollisionCallback callback(&remove);
      if (info.body0 && info.body1) {
        physicsWorld->contactPairTest(info.body0, info.body1, callback);
      }
      if (remove) {
        toRemove.push_back(&info);
      }
    }

    if (!toRemove.empty()) {
      for (CollisionInfo *info : toRemove) {
        collisions.erase(*info);
        world.event.dispatch(CollisionRemovedEvent(*info, world));
      }
    }
  }
}

} /* namespace simulation */
} /* namespace radix */
