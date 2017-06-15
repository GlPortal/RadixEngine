#include <radix/entities/traits/RigidBodyTrait.hpp>

#include <radix/simulation/Physics.hpp>
#include <radix/World.hpp>

const static std::string Tag = "RigidBodyTrait";

namespace radix {
namespace entities {

RigidBodyTrait::RigidBodyTrait() :
  body(nullptr) {
}

void RigidBodyTrait::setRigidBody(float mass,
  const std::shared_ptr<btCollisionShape> &collisionshape) {
  shape = collisionshape;
  motionState.setWorldTransform(*this);
  btVector3 localInertia;
  collisionshape->calculateLocalInertia(mass, localInertia);
  btRigidBody::btRigidBodyConstructionInfo ci(mass, &motionState, shape.get(), localInertia);
  body = new btRigidBody(ci);
  body->setUserPointer(static_cast<Entity*>(this));

  auto &phys = world.simulations.findFirstOfType<simulation::Physics>();
  Util::Log(Verbose, Tag) << "Adding body to phys world (" << id << ')';
  phys.getPhysicsWorld().addRigidBody(body);
}

RigidBodyTrait::~RigidBodyTrait() {
  delete body;
}

void RigidBodyTrait::setPosition(const Vector3f &val) {
  position = val;
  if (body) {
    btTransform t = body->getWorldTransform();
    t.setOrigin(val);
    body->setWorldTransform(t);
  }
}

void RigidBodyTrait::setScale(const Vector3f &val) {
  scale = val;
}

void RigidBodyTrait::setOrientation(const Quaternion &val) {
  orientation = val;
  if (body) {
    btTransform t = body->getWorldTransform();
    t.setRotation(val);
    body->setWorldTransform(t);
  }
}

} /* namespace entities */
} /* namespace radix */
