#ifndef RADIX_PHYSICS_GHOST_PAIR_CALLBACK_HPP
#define RADIX_PHYSICS_GHOST_PAIR_CALLBACK_HPP

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace radix {

class GhostPairCallback final: public btOverlappingPairCallback {
public:
  GhostPairCallback() {}
  ~GhostPairCallback() override {}

  btBroadphasePair* addOverlappingPair(
      btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) override;
  void* removeOverlappingPair(
      btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1, btDispatcher *dispatcher) override;
  void removeOverlappingPairsContainingProxy(
      btBroadphaseProxy *proxy, btDispatcher *dispatcher) override;
};

} /* namespace radix */

#endif /* RADIX_PHYSICS_GHOST_PAIR_CALLBACK_HPP */
