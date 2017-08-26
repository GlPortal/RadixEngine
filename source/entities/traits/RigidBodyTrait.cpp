#include <radix/entities/traits/RigidBodyTrait.hpp>

#include <radix/simulation/Physics.hpp>
#include <radix/World.hpp>

const static std::string Tag = "RigidBodyTrait";

namespace radix {
namespace entities {

RigidBodyTrait::RigidBodyTrait() :
  m_btPtrInfo(this, this),
  body(nullptr) {
}

int RigidBodyTrait::getCollisionFlags() const {
  return 0;
}

int RigidBodyTrait::getCollisionGroup() const {
  return 0;
}

int RigidBodyTrait::getCollisionMask() const {
  return 0;
}

void RigidBodyTrait::setRigidBody(float mass,
  const std::shared_ptr<btCollisionShape> &collisionshape) {
  shape = collisionshape;
  motionState.setWorldTransform(*this);
  btVector3 localInertia;
  collisionshape->calculateLocalInertia(mass, localInertia);
  btRigidBody::btRigidBodyConstructionInfo ci(mass, &motionState, shape.get(), localInertia);
  body = new btRigidBody(ci);
  body->setCollisionFlags(getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  body->setUserPointer(&m_btPtrInfo);
  body->setUserIndex(id);

  auto &phys = world.simulations.findFirstOfType<simulation::Physics>();
  Util::Log(Verbose, Tag) << "Adding body to phys world (" << id << ')';
  phys.getPhysicsWorld().addRigidBody(body, getCollisionGroup(), getCollisionMask());
}

RigidBodyTrait::~RigidBodyTrait() {
  if (body) {
    auto &phys = world.simulations.findFirstOfType<simulation::Physics>();
    Util::Log(Verbose, Tag) << "Removing body from phys world (" << id << ')';
    phys.getPhysicsWorld().removeRigidBody(body);
    delete body;
  }
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
