#ifndef COLLISION_DISPATCHER_HPP
#define COLLISION_DISPATCHER_HPP

#include <bullet/btBulletDynamicsCommon.h>

namespace radix {

class CollisionDispatcher : public btCollisionDispatcher {
public:
  using btCollisionDispatcher::btCollisionDispatcher;

  //bool needsCollision(const btCollisionObject*, const btCollisionObject*) final override;
  //bool needsResponse(const btCollisionObject*, const btCollisionObject*) final override;
};

} /* namespace radix */

#endif /* COLLISION_DISPATCHER_HPP */
