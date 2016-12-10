#include <radix/component/Trigger.hpp>
#include <radix/system/PhysicsSystem.hpp>

namespace radix {

Trigger::Trigger(Entity &ent, Action actionOnEnter, Action actionOnExit,
                 Action actionOnMove, Action actionOnUpdate)
  : Component(ent),
  actionOnEnter(actionOnEnter),
  actionOnExit(actionOnExit),
  actionOnMove(actionOnMove),
  actionOnUpdate(actionOnUpdate) {
  obj = new btGhostObject;
  Transform& tform = entity.getComponent<Transform>();
  obj->setWorldTransform(btTransform(tform.getOrientation(), tform.getPosition()));
  shape = std::make_shared<btCapsuleShape>(.4, 1);
  obj->setCollisionShape(shape.get());
  obj->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
  obj->setUserPointer(&entity);

  callbackOnEnter = entity.manager.world.event.addObserver(PhysicsSystem::
                                                           CollisionAddedEvent::Type,
  [this](const Event &event) {
    PhysicsSystem::CollisionAddedEvent& collisionAddedEvent
      = (PhysicsSystem::CollisionAddedEvent&) event;

    if (collisionAddedEvent.info.body1 == this->obj) {
      this->actionOnEnter(collisionAddedEvent.game);
    }
  });
  callbackOnExit = entity.manager.world.event.addObserver(PhysicsSystem::
                                                          CollisionRemovedEvent::Type,
  [this](const Event &event) {
    PhysicsSystem::CollisionRemovedEvent& collisionRemovedEvent
      = (PhysicsSystem::CollisionRemovedEvent&) event;

    if (collisionRemovedEvent.info.body1 == this->obj) {
      this->actionOnExit(collisionRemovedEvent.game);
    }
  });
}

Trigger::~Trigger() {
  delete obj;
}

} /* namespace radix */