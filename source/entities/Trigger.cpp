#include <radix/entities/Trigger.hpp>

#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>

#include <radix/BaseGame.hpp>
#include <radix/simulation/Physics.hpp>

namespace radix {
namespace entities {

Trigger::Trigger(const CreationParams &cp)
  : Entity(cp),
  actionOnEnter([] (Trigger&) {}),
  actionOnExit([] (Trigger&) {}),
  actionOnMove([] (Trigger&) {}),
  actionOnUpdate([] (Trigger&) {}) {
  ghostObject = new btGhostObject;
  ghostObject->setWorldTransform(btTransform(getOrientation(), getPosition()));
  shape = std::make_shared<btBoxShape>(btVector3(getScale().x/2, getScale().y/2, getScale().z/2));
  ghostObject->setCollisionShape(shape.get());
  ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
  ghostObject->setUserPointer(this);

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
}

btGhostObject* Trigger::getBulletGhostObject(){
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

Trigger::~Trigger() {
  delete ghostObject;
}

} /* namespace entities */
} /* namespace radix */
