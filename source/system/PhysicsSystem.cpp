#include <radix/system/PhysicsSystem.hpp>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <radix/component/Player.hpp>
#include <radix/component/RigidBody.hpp>
#include <radix/physics/CollisionDispatcher.hpp>
#include <radix/physics/Uncollider.hpp>
#include <radix/World.hpp>

namespace radix {

PhysicsSystem::PhysicsSystem(World &world) :
  System(world),
  filterCallback(nullptr),
  broadphase(new btDbvtBroadphase),
  collisionConfiguration(new btDefaultCollisionConfiguration()),
  dispatcher(new CollisionDispatcher(collisionConfiguration)),
  solver(new btSequentialImpulseConstraintSolver),
  physWorld(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration)),
  gpCallback(new btGhostPairCallback) {
  broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(gpCallback);
  filterCallback = new Uncollider(world);
  //physWorld->getPairCache()->setOverlapFilterCallback(filterCallback);
  dispatcher->setNearCallback(Uncollider::nearCallback);
  physWorld->setGravity(btVector3(0, -9.8, 0));

  cbCompAdd = world.event.observe(Entity::ComponentAddedEvent::Type, [this](const radix::Event &e) {
    Component &component = ((Entity::ComponentAddedEvent&)e).component;
    auto componentId = component.getTypeId();
    if (componentId == Component::getTypeId<RigidBody>()) {
      RigidBody &rb = (RigidBody&)component;
      this->physWorld->addRigidBody(rb.body);
    } else if (componentId == Component::getTypeId<Player>()) {
      Player &p = (Player&)component;
      this->physWorld->addCollisionObject(p.obj,
        btBroadphaseProxy::CharacterFilter,
        btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
      this->physWorld->addAction(p.controller);
    }
  });
  cbCompRem = world.event.observe(Entity::ComponentRemovedEvent::Type, [this](const radix::Event &event) {
    Component &component = ((Entity::ComponentAddedEvent&)event).component;
    auto componentId = component.getTypeId();
    if (componentId == Component::getTypeId<RigidBody>()) {
      RigidBody &rigidBody = (RigidBody&)component;
      this->physWorld->removeRigidBody(rigidBody.body);
    } else if (componentId == Component::getTypeId<Player>()) {
      Player &player = (Player&)component;
      this->physWorld->removeAction(player.controller);
      this->physWorld->removeCollisionObject(player.obj);
    }
  });

  for (Entity &entity : world.entities) {
    if (entity.hasComponent<RigidBody>()) {
      cbCompAdd(Entity::ComponentAddedEvent(entity, entity.getComponent<RigidBody>()));
    }
    if (entity.hasComponent<Player>()) {
      cbCompAdd(Entity::ComponentAddedEvent(entity, entity.getComponent<Player>()));
    }
  }
}

PhysicsSystem::~PhysicsSystem() {
  delete filterCallback;
}

void PhysicsSystem::update(TDelta timeDelta) {
  filterCallback->beforePhysicsStep();
  physWorld->stepSimulation(timeDelta.sec_d(), 10);
  for (Entity &entity : world.entities) {
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
}

} /* namespace radix */
