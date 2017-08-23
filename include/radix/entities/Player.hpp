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

namespace radix {
namespace entities {

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

  Trigger *trigger;

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

class ContactPlayerCallback : public btCollisionWorld::ContactResultCallback {
public:
  virtual btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,
                                   int partId0, int index0,const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
    Entity* playerEntity = (Entity*) colObj0Wrap->getCollisionObject()->getUserPointer();
    Entity* triggerEntity = (Entity*) colObj1Wrap->getCollisionObject()->getUserPointer();

    if (triggerEntity && playerEntity) {
      Trigger *trigger = dynamic_cast<Trigger*>(triggerEntity);
      if (!trigger) {
        return 0;
      }
      trigger->onUpdate();

      Player *player = dynamic_cast<Player*>(playerEntity);
      if (player) {
        player->trigger = trigger;
      }
    }
    return 0;
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_PLAYER_HPP */
