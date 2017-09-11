#include <radix/entities/Trigger.hpp>

#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>

#include <radix/BaseGame.hpp>
#include <radix/simulation/Physics.hpp>

static const char *Tag = "Trigger";

namespace radix {
namespace entities {

Trigger::Trigger(const CreationParams &cp) :
  Entity(cp),
  m_btGpCallbacks(this),
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
  ghostObject->setUserPointer(&m_btGpCallbacks);
  ghostObject->setUserIndex(id);

  m_btGpCallbacks.onEnter = [this](const util::BulletGhostPairCallbacks::CallbackParams &params) {
    this->actionOnEnter(*this);
  };
  m_btGpCallbacks.onExit = [this](const util::BulletGhostPairCallbacks::CallbackParams &params) {
    this->actionOnExit(*this);
  };
  auto &physWorld = world.simulations.findFirstOfType<simulation::Physics>().getPhysicsWorld();
  Util::Log(Verbose, Tag) << "Adding trigger to phys world (" << id << ')';
  physWorld.addCollisionObject(ghostObject,
      btBroadphaseProxy::SensorTrigger,
      btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter);

  /*btRigidBody *obj = new btRigidBody(btRigidBodyConstructionInfo);
  obj->setWorldTransform(btTransform(getOrientation(), getPosition()));
  obj->setCollisionShape(shape.get());
  obj->setCollisionFlags(
      btCollisionObject::CF_STATIC_OBJECT |
      btCollisionObject::CF_NO_CONTACT_RESPONSE |
      btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  obj->setUserPointer(&m_btPtrInfo);
  obj->setUserIndex(id);
  physWorld.addRigidBody(obj,
      btBroadphaseProxy::SensorTrigger,
      btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter);*/
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

void Trigger::setPosition(const Vector3f &pos) {
  position = pos;
  ghostObject->setWorldTransform(*this);
}

} /* namespace entities */
} /* namespace radix */
