#ifndef RADIX_UNCOLLIDER_HPP
#define RADIX_UNCOLLIDER_HPP

#include <list>
#include <unordered_set>

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>

namespace std {
template<typename T1, typename T2>
struct hash<std::pair<T1*, T2*>> {
  std::size_t operator()(std::pair<T1*, T2*> const &p) const {
    return (std::size_t)((p.second - p.first) * ((intptr_t)p.first ^ 0x412c765140f2fa32) + (intptr_t)p.second);
  }
};
}

namespace radix {

class World;

class Uncollider : public btOverlapFilterCallback {
private:
  World &world;

public:
  static std::list<btCollisionObject*> volumes;
  static std::unordered_set<std::pair<btCollisionObject*, btCollisionObject*>>
    collisonPairExclusions;
  static void addCollisonPairExclusion(btCollisionObject*, btCollisionObject*);
  static void removeCollisonPairExclusion(btCollisionObject*, btCollisionObject*);
  static bool isPointInUncollideVolume(const btVector3 &p);

  Uncollider(World&);
  void beforePhysicsStep();
  bool needBroadphaseCollision(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) const;
  static void nearCallback(btBroadphasePair&, btCollisionDispatcher&, const btDispatcherInfo&);
};

} /* namespace radix */

#endif /* RADIX_UNCOLLIDER_HPP */
