#include <radix/entities/Trigger.hpp>

#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>

#include <radix/BaseGame.hpp>
#include <radix/simulation/Physics.hpp>

const static std::string Tag = "Trigger";

namespace radix {
namespace entities {

Trigger::Trigger(const CreationParams &cp) :
  Entity(cp),
  m_btPtrInfo(this),
  actionOnEnter([] (Trigger&) {}),
  actionOnExit([] (Trigger&) {}),
  actionOnMove([] (Trigger&) {}),
  actionOnUpdate([] (Trigger&) {}) {
  ghostObject = new btGhostObject;
  ghostObject->setWorldTransform(btTransform(getOrientation(), getPosition()));
  shape = std::make_shared<btBoxShape>(btVector3(getScale().x/2, getScale().y/2, getScale().z/2));
  ghostObject->setCollisionShape(shape.get());
  ghostObject->setCollisionFlags(
      btCollisionObject::CF_STATIC_OBJECT |
      btCollisionObject::CF_NO_CONTACT_RESPONSE |
      btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  ghostObject->setUserPointer(&m_btPtrInfo);
  ghostObject->setUserIndex(id);

  callbackOnEnter = world.event.addObserver(simulation::Physics::
                                                           CollisionAddedEvent::Type,
  [this](const Event &event) {
    simulation::Physics::CollisionAddedEvent& collisionAddedEvent
      = (simulation::Physics::CollisionAddedEvent&) event;

    if (collisionAddedEvent.info.body1 == this->ghostObject) {
      this->actionOnEnter(*this);
    }
  });
  callbackOnExit = world.event.addObserver(simulation::Physics::
                                                          CollisionRemovedEvent::Type,
  [this](const Event &event) {
    simulation::Physics::CollisionRemovedEvent& collisionRemovedEvent
      = (simulation::Physics::CollisionRemovedEvent&) event;

    if (collisionRemovedEvent.info.body1 == this->ghostObject) {
      this->actionOnExit(*this);
    }
  });
  auto &physWorld = world.simulations.findFirstOfType<simulation::Physics>().getPhysicsWorld();
  Util::Log(Verbose, Tag) << "Adding trigger to phys world (" << id << ')';
  physWorld.addCollisionObject(ghostObject,
      btBroadphaseProxy::SensorTrigger,
      btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter);
}

Trigger::~Trigger() {
  auto &physWorld = world.simulations.findFirstOfType<simulation::Physics>().getPhysicsWorld();
  Util::Log(Verbose, Tag) << "Removing trigger from phys world (" << id << ')';
  physWorld.removeCollisionObject(ghostObject);
  delete ghostObject;
}

btGhostObject* Trigger::getBulletGhostObject() {
  return ghostObject;
}

void Trigger::setActionOnEnter(Action action){
  actionOnEnter = action;
}

void Trigger::setActionOnMove(Action action){
  actionOnMove = action;
}

void Trigger::setActionOnUpdate(Action action){
  actionOnUpdate = action;
}

void Trigger::setActionOnExit(Action action){
  actionOnExit = action;
}

} /* namespace entities */
} /* namespace radix */
