#include <radix/component/Trigger.hpp>
#include <radix/system/PhysicsSystem.hpp>

namespace radix {

Trigger::Trigger(Entity &ent)
  : Component(ent),
  actionOnEnter([] (BaseGame& game) {}),
  actionOnExit([] (BaseGame& game) {}),
  actionOnMove([] (BaseGame& game) {}),
  actionOnUpdate([] (BaseGame& game) {}) {
  ghostObject = new btGhostObject;
  Transform& tform = entity.getComponent<Transform>();
  ghostObject->setWorldTransform(btTransform(tform.getOrientation(), tform.getPosition()));
  shape = std::make_shared<btBoxShape>(btVector3(tform.getScale().x/2, tform.getScale().y/2, tform.getScale().z/2));
  ghostObject->setCollisionShape(shape.get());
  ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
  ghostObject->setUserPointer(&entity);

  callbackOnEnter = entity.manager.world.event.addObserver(PhysicsSystem::
                                                           CollisionAddedEvent::Type,
  [this](const Event &event) {
    PhysicsSystem::CollisionAddedEvent& collisionAddedEvent
      = (PhysicsSystem::CollisionAddedEvent&) event;

    if (collisionAddedEvent.info.body1 == this->ghostObject) {
      this->actionOnEnter(collisionAddedEvent.game);
    }
  });
  callbackOnExit = entity.manager.world.event.addObserver(PhysicsSystem::
                                                          CollisionRemovedEvent::Type,
  [this](const Event &event) {
    PhysicsSystem::CollisionRemovedEvent& collisionRemovedEvent
      = (PhysicsSystem::CollisionRemovedEvent&) event;

    if (collisionRemovedEvent.info.body1 == this->ghostObject) {
      this->actionOnExit(collisionRemovedEvent.game);
    }
  });
}

btGhostObject *Trigger::getBulletGhostObject(){
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

} /* namespace radix */
