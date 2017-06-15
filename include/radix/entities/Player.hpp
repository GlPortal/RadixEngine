#ifndef RADIX_ENTITIES_PLAYER_HPP
#define RADIX_ENTITIES_PLAYER_HPP

#include <array>

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletCollision/CollisionShapes/btConvexShape.h>

#include <radix/entities/traits/HealthTrait.hpp>
#include <radix/entities/traits/SoundSourceTrait.hpp>
#include <radix/Entity.hpp>
#include <radix/physics/KinematicCharacterController.hpp>

namespace radix {
namespace entities {

class Trigger;

class Player :
    public virtual Entity,
    public HealthTrait,
    public SoundSourceTrait {
public:
  std::shared_ptr<btConvexShape> shape;
  btPairCachingGhostObject *obj;
  KinematicCharacterController *controller;

  Vector3f velocity, headAngle;
  bool flying, noclip, frozen;
  float speed;
  float stepCounter;

  entities::Trigger *trigger;

  Player(const CreationParams&);
  ~Player();

  void tick(TDelta) override;

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
