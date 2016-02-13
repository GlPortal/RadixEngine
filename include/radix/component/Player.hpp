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
#include <radix/scene/Scene.hpp>

namespace glPortal {

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

  Quaternion getBaseHeadOrientation() const;
  Quaternion getHeadOrientation() const;
};

} /* namespace glPortal */

#endif /* COMPONENT_PLAYER_HPP */
