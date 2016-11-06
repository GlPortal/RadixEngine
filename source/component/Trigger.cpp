#include <radix/component/Trigger.hpp>
#include <radix/component/Transform.hpp>

namespace radix {

Trigger::Trigger(Entity &ent) : Component(ent) {
  obj = new btGhostObject;
  Transform& tform = entity.getComponent<Transform>();
  obj->setWorldTransform(btTransform(tform.getOrientation(), tform.getPosition()));
  shape = std::make_shared<btCapsuleShape>(.4, 1);
  obj->setCollisionShape(shape.get());
  obj->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
  obj->setUserPointer(&entity);
}

Trigger::~Trigger() {
  delete obj;
}

} /* namespace radix */