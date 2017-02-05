#ifndef RADIX_COLLISION_DISPATCHER_HPP
#define RADIX_COLLISION_DISPATCHER_HPP

#include <btBulletDynamicsCommon.h>

namespace radix {

class CollisionDispatcher : public btCollisionDispatcher {
public:
  using btCollisionDispatcher::btCollisionDispatcher;

  //bool needsCollision(const btCollisionObject*, const btCollisionObject*) final override;
  //bool needsResponse(const btCollisionObject*, const btCollisionObject*) final override;
};

} /* namespace radix */

#endif /* RADIX_COLLISION_DISPATCHER_HPP */
