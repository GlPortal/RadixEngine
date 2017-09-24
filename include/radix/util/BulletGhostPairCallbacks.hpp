#ifndef RADIX_UTIL_BULLET_GHOST_PAIR_CALLBACKS_HPP
#define RADIX_UTIL_BULLET_GHOST_PAIR_CALLBACKS_HPP

#include <functional>

#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <radix/util/BulletUserPtrInfo.hpp>

namespace radix {
namespace util {

struct BulletGhostPairCallbacks : public BulletUserPtrInfo {
  struct CallbackParams {
    const BulletGhostPairCallbacks &gpCallbacks;
    btGhostObject *ghostObj;
    btCollisionObject *colObj;

    inline CallbackParams(
        const BulletGhostPairCallbacks &gpc, btGhostObject *go, btCollisionObject *co) :
      gpCallbacks(gpc),
      ghostObj(go),
      colObj(co) {
    }
  };

  std::function<void(const CallbackParams&)> onEnter, onExit;


  inline BulletGhostPairCallbacks(Entity *e = nullptr, entities::Trait *t = nullptr) :
    BulletUserPtrInfo(e, t) {
  }
};

} /* namespace util */
} /* namespace radix */

#endif /* RADIX_UTIL_BULLET_GHOST_PAIR_CALLBACKS_HPP */
