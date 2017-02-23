#ifndef RADIX_COMPONENT_RIGIDBODY_HPP
#define RADIX_COMPONENT_RIGIDBODY_HPP

#include <memory>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

#include <radix/component/Component.hpp>
#include <radix/component/Transform.hpp>
#include <radix/Entity.hpp>
#include <radix/EntityManager.hpp>

namespace radix {

class RigidBody : public Component {
public:
  std::shared_ptr<btCollisionShape> shape;
  btDefaultMotionState motionState;
  btRigidBody *body;

  RigidBody(Entity &ent);
  RigidBody(Entity &ent, float mass, const std::shared_ptr<btCollisionShape> &collisionshape);
  ~RigidBody();

  const char* getName() const {
    return "RigidBody";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver&);
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_RIGIDBODY_HPP */
