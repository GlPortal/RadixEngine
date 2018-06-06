#ifndef RADIX_ENTITIES_PLAYER_HPP
#define RADIX_ENTITIES_PLAYER_HPP

#include <array>

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletCollision/CollisionShapes/btConvexShape.h>

#include <radix/entities/traits/HealthTrait.hpp>
#include <radix/entities/traits/SoundSourceTrait.hpp>
#include <radix/Entity.hpp>
#include <radix/physics/KinematicCharacterController.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/util/BulletGhostPairCallbacks.hpp>

namespace radix {

class Vector2f;

namespace entities {

class Player :
    public virtual Entity,
    public HealthTrait,
    public SoundSourceTrait {
protected:
  util::BulletGhostPairCallbacks m_btGpCallbacks;

public:
  std::shared_ptr<btConvexShape> shape;
  btPairCachingGhostObject *obj;
  KinematicCharacterController *controller;

  Vector3f velocity, headAngle;
  bool flying, noclip, frozen, attemptJump;
  float speed;
  float stepCounter;
  Vector3f newMovement, oldMovement, headingChange;

  Trigger *trigger;

  Player(const CreationParams&);
  ~Player();

  void tick(TDelta) override;
  void jump();
  void move(const Vector2f &move);
  void moveX(const float &move);
  void moveY(const float &move);
  void changeHeading(const Vector2f& lookVector);

  Quaternion getBaseHeadOrientation() const;
  Quaternion getHeadOrientation() const;
  inline void setHeadOrientation(Quaternion &quaternion) {
    headAngle = quaternion.toAero();
  }

  virtual void setPosition(const Vector3f&) override;

  std::string fullClassName() const override {
    return "radix/entities/Player";
  }
  std::string className() const override {
    return "Player";
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_PLAYER_HPP */
