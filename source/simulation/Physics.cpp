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
      if (typeid(e.entity) == typeid(Player)) {
        Util::Log(Verbose, Tag) << "Adding Player(" << e.entity.id << ')';
        Player &player = dynamic_cast<Player&>(e.entity);
        physicsWorld->addCollisionObject(player.obj,
            btBroadphaseProxy::CharacterFilter,
            btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
        physicsWorld->addAction(player.controller);
      } else if (typeid(e.entity) == typeid(Trigger)) {
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
      if (typeid(e.entity) == typeid(Player)) {
        Player &player = dynamic_cast<Player&>(e.entity);
        physicsWorld->removeAction(player.controller);
        physicsWorld->removeCollisionObject(player.obj);
      } else {
        RigidBodyTrait *rbtp = dynamic_cast<RigidBodyTrait*>(&e.entity);
        if (rbtp) {
          physicsWorld->removeRigidBody(rbtp->body);
        }
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

  ContactPlayerCallback callback(*game);
  physicsWorld->contactTest(world.getPlayer().obj, callback);
  checkCollisions();
}

bool Physics::contactProcessedCallback(btManifoldPoint &cp, void *body0, void *body1) {
  CollisionInfo pair((btCollisionObject*) body0, (btCollisionObject*) body1);
  if (!collisions.empty()) {
    auto found = collisions.find(pair);
    if (found == collisions.end()) {
      collisions.insert(pair);
      instance->world.event.dispatch(CollisionAddedEvent(pair, *instance->game));
    }
  } else {
    collisions.insert(pair);
  }
  cp.m_userPersistentData = (void*) &*collisions.find(pair);
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
        world.event.dispatch(CollisionRemovedEvent(*info, *game));
      }
    }
  }
}

} /* namespace simulation */
} /* namespace radix */
