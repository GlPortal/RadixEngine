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
  cbCompRem = world.event.observe(Entity::ComponentRemovedEvent::Type, [this](const radix::Event &e) {
    Component &component = ((Entity::ComponentAddedEvent&)e).component;
    auto componentId = component.getTypeId();
    if (componentId == Component::getTypeId<RigidBody>()) {
      RigidBody &rb = (RigidBody&)component;
      this->physWorld->removeRigidBody(rb.body);
    } else if (componentId == Component::getTypeId<Player>()) {
      Player &p = (Player&)component;
      this->physWorld->removeAction(p.controller);
      this->physWorld->removeCollisionObject(p.obj);
    }
  });

  for (Entity &e : world.entities) {
    if (e.hasComponent<RigidBody>()) {
      cbCompAdd(Entity::ComponentAddedEvent(e, e.getComponent<RigidBody>()));
    }
    if (e.hasComponent<Player>()) {
      cbCompAdd(Entity::ComponentAddedEvent(e, e.getComponent<Player>()));
    }
  }
}

PhysicsSystem::~PhysicsSystem() {
  delete filterCallback;
}

void PhysicsSystem::update(TDelta dtime) {
  filterCallback->beforePhysicsStep();
  physWorld->stepSimulation(dtime.sec_d(), 10);
  for (Entity &e : world.entities) {
    if (e.hasComponent<RigidBody>()) {
      RigidBody &b = e.getComponent<RigidBody>();
      if (not b.body->isStaticObject()) {
        Transform &t = e.getComponent<Transform>();
        btTransform btTform;
        b.body->getMotionState()->getWorldTransform(btTform);
        t.privSetPosition(btTform.getOrigin());
        t.privSetOrientation(btTform.getRotation());
      }
    }
  }
}

} /* namespace radix */
