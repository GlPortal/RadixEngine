#ifndef COMPONENT_PLAYER_HPP
#define COMPONENT_PLAYER_HPP

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

#include <radix/component/Component.hpp>
#include <radix/component/Transform.hpp>
#include <radix/Entity.hpp>
#include <radix/EntityManager.hpp>
#include <radix/physics/KinematicCharacterController.hpp>

namespace radix {

class Player : public Component {
public:
  std::shared_ptr<btConvexShape> shape;
  btPairCachingGhostObject *obj;
  KinematicCharacterController *controller;

  Vector3f velocity, headAngle;
  bool flying, noclip, frozen;
  float speed;
  float stepCounter;

  Player(Entity&);
  ~Player();

  const char* getName() const {
    return "Player";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver&);

  Quaternion getBaseHeadOrientation() const;
  Quaternion getHeadOrientation() const;
};

} /* namespace radix */

#endif /* COMPONENT_PLAYER_HPP */
