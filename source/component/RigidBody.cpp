#include <radix/component/RigidBody.hpp>

#include <ciso646>

namespace radix {

RigidBody::RigidBody(Entity &ent) :
  Component(ent),
  body(nullptr) {
}

RigidBody::RigidBody(Entity &ent, float mass,
  const std::shared_ptr<btCollisionShape> &collisionshape) :
  Component(ent), shape(collisionshape) {
  if (not entity.hasComponent<Transform>()) {
    entity.addComponent<Transform>();
  }
  Transform &tform = entity.getComponent<Transform>();
  motionState.setWorldTransform(btTransform(tform.getOrientation(), tform.getPosition()));
  btVector3 localInertia;
  collisionshape->calculateLocalInertia(mass, localInertia);
  btRigidBody::btRigidBodyConstructionInfo ci(mass, &motionState, shape.get(), localInertia);
  body = new btRigidBody(ci);
  body->setUserPointer(&entity);
}

RigidBody::~RigidBody() {
  delete body;
}

void RigidBody::serialize(serine::Archiver &ar) {
  /// @todo RigidBody serialization
}

} /* namespace radix */
