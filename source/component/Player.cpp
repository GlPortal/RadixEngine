#include <radix/component/Player.hpp>

#ifdef _WIN32
#include <ciso646>
#endif

namespace radix {

static const Vector3f PLAYER_SIZE(0.5, 1, 0.5);

Player::Player(Entity &ent) :
  Component(ent),
  flying(false),
  noclip(false),
  frozen(false),
  stepCounter(0) {

  /// @todo Move this to systems and add a dependency model
  if (not entity.hasComponent<Transform>()) {
    entity.addComponent<Transform>();
  }
  entity.getComponent<Transform>().setScale(PLAYER_SIZE);

  obj = new btPairCachingGhostObject;
  Transform &tform = entity.getComponent<Transform>();
  obj->setWorldTransform(btTransform(tform.getOrientation(), tform.getPosition()));
  shape = std::make_shared<btCapsuleShape>(.4, 1);
  obj->setCollisionShape(shape.get());
  obj->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
  obj->setUserPointer(&entity);
  controller = new KinematicCharacterController(obj, shape.get(), 0.35);
}

Player::~Player() {
  delete controller;
  delete obj;

  for (std::pair<int, PlayerTask*> pairs : tasks) {
    delete pairs.second;
  }
}

void Player::serialize(serine::Archiver &ar) {

}

void Player::removeTask(int id) {
  delete tasks.at(id);
  tasks.erase(id);
}

Quaternion Player::getBaseHeadOrientation() const {
  return Quaternion().fromAero(headAngle);
}

Quaternion Player::getHeadOrientation() const {
  return Quaternion().fromAero(headAngle);
}

} /* namespace radix */
