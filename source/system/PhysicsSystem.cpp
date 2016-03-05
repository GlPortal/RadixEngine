#include <radix/system/PhysicsSystem.hpp>

#include <radix/component/Player.hpp>
#include <radix/component/RigidBody.hpp>
#include <radix/physics/Uncollider.hpp>
#include <radix/World.hpp>

namespace radix {

PhysicsSystem::PhysicsSystem(World &w) :
  System(w),
  filterCallback(nullptr),
  broadphase(new btDbvtBroadphase),
  collisionConfiguration(new btDefaultCollisionConfiguration()),
  dispatcher(new CollisionDispatcher(collisionConfiguration)),
  solver(new btSequentialImpulseConstraintSolver),
  physWorld(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration)),
  gpCallback(new btGhostPairCallback) {
  broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(gpCallback);
  filterCallback = new Uncollider(w);
  //physWorld->getPairCache()->setOverlapFilterCallback(filterCallback);
  dispatcher->setNearCallback(Uncollider::nearCallback);
  physWorld->setGravity(btVector3(0, -9.8, 0));

  cpCompAdd = w.event.observe(Entity::ComponentAddedEvent::Type, [this](const radix::Event &e){
    Component &c = ((Entity::ComponentAddedEvent&)e).component;
    auto ctid = c.getTypeId();
    if (ctid == Component::getTypeId<RigidBody>()) {
      RigidBody &rb = (RigidBody&)c;
      this->physWorld->addRigidBody(rb.body);
    } else if (ctid == Component::getTypeId<Player>()) {
      Player &p = (Player&)c;
      this->physWorld->addCollisionObject(p.obj,
        btBroadphaseProxy::CharacterFilter,
        btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
      this->physWorld->addAction(p.controller);
    }
  });
  cpCompRem = w.event.observe(Entity::ComponentRemovedEvent::Type, [this](const radix::Event &e){
    Component &c = ((Entity::ComponentAddedEvent&)e).component;
    auto ctid = c.getTypeId();
    if (ctid == Component::getTypeId<RigidBody>()) {
      RigidBody &rb = (RigidBody&)c;
      this->physWorld->removeRigidBody(rb.body);
    } else if (ctid == Component::getTypeId<Player>()) {
      Player &p = (Player&)c;
      this->physWorld->removeAction(p.controller);
      this->physWorld->removeCollisionObject(p.obj);
    }
  });
}

PhysicsSystem::~PhysicsSystem() {
  world.event.unobserve(cpCompAdd, cpCompRem);
  delete filterCallback;
}

void PhysicsSystem::update(float dtime) {
  filterCallback->beforePhysicsStep();
  physWorld->stepSimulation(dtime, 10);
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
