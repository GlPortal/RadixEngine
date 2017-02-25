#include <radix/system/PhysicsSystem.hpp>

#include <radix/component/Player.hpp>
#include <radix/component/RigidBody.hpp>
#include <radix/physics/CollisionDispatcher.hpp>
#include <radix/physics/Uncollider.hpp>

namespace radix {

std::unordered_set<CollisionInfo, CollisionInfoHash, CollisionInfoEqual> PhysicsSystem::collisions;
PhysicsSystem* PhysicsSystem::instance;

PhysicsSystem::PhysicsSystem(World &world, BaseGame *game) :
  System(world),
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
  physicsWorld->setGravity(btVector3(0, -9.8, 0));

  gContactProcessedCallback = reinterpret_cast<ContactProcessedCallback>
  (&PhysicsSystem::contactProcessedCallback);

  cbCompAdd = world.event.addObserver(Entity::ComponentAddedEvent::Type,
                                      [this](const radix::Event &e) {
      Component &component = ((Entity::ComponentAddedEvent &) e).component;
      auto componentId = component.getTypeId();
      if (componentId == Component::getTypeId<RigidBody>()) {
        RigidBody &rb = (RigidBody &) component;
        this->physicsWorld->addRigidBody(rb.body);
      } else if (componentId == Component::getTypeId<Player>()) {
        Player &player = (Player &) component;
        this->physicsWorld->addCollisionObject(player.obj,
                                               btBroadphaseProxy::CharacterFilter,
                                               btBroadphaseProxy::StaticFilter |
                                               btBroadphaseProxy::DefaultFilter);
        this->physicsWorld->addAction(player.controller);
      } else if (componentId == Component::getTypeId<Trigger>()) {
        Trigger &trigger = (Trigger&) component;
        trigger.getBulletGhostObject()->setCollisionFlags
        (
         trigger.getBulletGhostObject()->getCollisionFlags() |
         btCollisionObject::CF_NO_CONTACT_RESPONSE
         );
        this->physicsWorld->addCollisionObject(trigger.getBulletGhostObject());
      }
    });
  cbCompRem = world.event.addObserver(Entity::ComponentRemovedEvent::Type,
                                      [this](const radix::Event &event) {
      Component &component = ((Entity::ComponentAddedEvent &) event).component;
      auto componentId = component.getTypeId();
      if (componentId == Component::getTypeId<RigidBody>()) {
        RigidBody &rigidBody = (RigidBody &) component;
        this->physicsWorld->removeRigidBody(rigidBody.body);
      } else if (componentId == Component::getTypeId<Player>()) {
        Player &player = (Player &) component;
        this->physicsWorld->removeAction(player.controller);
        this->physicsWorld->removeCollisionObject(player.obj);
      }
    });

  for (Entity &entity : world.entityManager) {
    if (entity.hasComponent<RigidBody>()) {
      cbCompAdd(Entity::ComponentAddedEvent(entity, entity.getComponent<RigidBody>()));
    }
    if (entity.hasComponent<Player>()) {
      cbCompAdd(Entity::ComponentAddedEvent(entity, entity.getComponent<Player>()));
    }
  }

  instance = this;
}

PhysicsSystem::~PhysicsSystem() {
  delete filterCallback;
}

void PhysicsSystem::update(TDelta timeDelta) {
  filterCallback->beforePhysicsStep();
  physicsWorld->stepSimulation(timeDelta.sec_d(), 10);
  for (Entity &entity : world.entityManager) {
    if (entity.hasComponent<RigidBody>()) {
      RigidBody &rigidBody = entity.getComponent<RigidBody>();
      if (not rigidBody.body->isStaticObject()) {
        Transform &transform = entity.getComponent<Transform>();
        btTransform btTform;
        rigidBody.body->getMotionState()->getWorldTransform(btTform);
        transform.privSetPosition(btTform.getOrigin());
        transform.privSetOrientation(btTform.getRotation());
      }
    }
  }

  ContactPlayerCallback callback(*game);
  physicsWorld->contactTest(world.getPlayer().getComponent<Player>().obj, callback);
  checkCollisions();
}

bool PhysicsSystem::contactProcessedCallback(btManifoldPoint &cp, void *body0, void *body1) {
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

void PhysicsSystem::checkCollisions() {
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

} /* namespace radix */
