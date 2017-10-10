#ifndef RADIX_ENTITIES_RIGID_BODY_TRAIT_HPP
#define RADIX_ENTITIES_RIGID_BODY_TRAIT_HPP

#include <memory>

#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <bullet/LinearMath/btDefaultMotionState.h>

#include <radix/entities/traits/Trait.hpp>
#include <radix/util/BulletUserPtrInfo.hpp>

namespace radix {
namespace entities {

class RigidBodyTrait : public Trait {
protected:
  util::BulletUserPtrInfo m_btPtrInfo;

  RigidBodyTrait();

public:
  std::shared_ptr<btCollisionShape> shape;
  btDefaultMotionState motionState;
  btRigidBody *body;

  virtual void onRemoveTrait() override;
  ~RigidBodyTrait();

  virtual int getCollisionFlags() const;
  virtual int getCollisionGroup() const;
  virtual int getCollisionMask() const;
  void setRigidBody(float mass,
    const std::shared_ptr<btCollisionShape> &collisionshape);

  virtual void setPosition(const Vector3f&) override;
  virtual void setScale(const Vector3f&) override;
  virtual void setOrientation(const Quaternion&) override;
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_TRAITS_RIGID_BODY_TRAIT_HPP */
