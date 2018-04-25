#include <radix/entities/Trigger.hpp>

#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>

#include <radix/BaseGame.hpp>
#include <radix/simulation/Physics.hpp>
#include <radix/World.hpp>

static const char *Tag = "Trigger";

namespace radix {
namespace entities {

Trigger::Trigger(const CreationParams &cp) :
  Trigger(cp, {}) {
}

Trigger::Trigger(const CreationParams &cp, const Transform &tform) :
  Entity(cp),
  bulletGhostPairCallbacks(this),
  actionOnEnter([] (Trigger&) {}),
  actionOnExit([] (Trigger&) {}),
  actionOnMove([] (Trigger&) {}),
  actionOnUpdate([] (Trigger&) {}) {
  static_cast<Transform&>(*this) = tform;
  ghostObject = std::make_unique<btGhostObject>();
  ghostObject->setWorldTransform(btTransform(getOrientation(), getPosition()));
  shape = std::make_unique<btBoxShape>(btVector3(.5, .5, .5));
  shape->setLocalScaling(scale);
  ghostObject->setCollisionShape(shape.get());
  ghostObject->setCollisionFlags(
      btCollisionObject::CF_STATIC_OBJECT |
      btCollisionObject::CF_NO_CONTACT_RESPONSE |
      btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  ghostObject->setUserPointer(&bulletGhostPairCallbacks);
  ghostObject->setUserIndex(id);

  bulletGhostPairCallbacks.onEnter = [this](const util::BulletGhostPairCallbacks::CallbackParams &params) {
    this->actionOnEnter(*this);
  };
  bulletGhostPairCallbacks.onExit = [this](const util::BulletGhostPairCallbacks::CallbackParams &params) {
    this->actionOnExit(*this);
  };
  auto &physWorld = world.simulations.findFirstOfType<simulation::Physics>().getPhysicsWorld();
  Util::Log(Verbose, Tag) << "Adding trigger to phys world (" << id << ')';
  physWorld.addCollisionObject(ghostObject.get(),
      btBroadphaseProxy::SensorTrigger,
      btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter);
}

Trigger::~Trigger() {
  auto &physWorld = world.simulations.findFirstOfType<simulation::Physics>().getPhysicsWorld();
  Util::Log(Verbose, Tag) << "Removing trigger from phys world (" << id << ')';
  physWorld.removeCollisionObject(ghostObject.get());
}

btGhostObject* Trigger::getBulletGhostObject() const {
  return ghostObject.get();
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

void Trigger::setOrientation(const Quaternion &q) {
  orientation = q;
  ghostObject->setWorldTransform(*this);
}

void Trigger::setScale(const Vector3f &s) {
  scale = s;
  shape->setLocalScaling(scale);
  world.simulations.findFirstOfType<simulation::Physics>().getPhysicsWorld()
      .updateSingleAabb(ghostObject.get());
}

} /* namespace entities */
} /* namespace radix */
