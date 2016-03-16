#ifndef RADIX_COLLISION_SHAPE_MANAGER_HPP
#define RADIX_COLLISION_SHAPE_MANAGER_HPP

#include <radix/core/math/Vector3f.hpp>
#include <radix/Entity.hpp>

namespace radix {

class CollisionShapeManager {
public:
  BoxCollider();
  BoxCollider(const Vector3f &position, const Vector3f &size);
  bool collidesWith(const BoxCollider &collider) const;
  static BoxCollider generateCage(const Entity &entity);

  Vector3f position;
  Vector3f size;
};

} /* namespace radix */

#endif /* RADIX_COLLISION_SHAPE_MANAGER_HPP */
