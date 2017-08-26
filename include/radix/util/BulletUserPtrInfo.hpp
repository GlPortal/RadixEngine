#ifndef RADIX_UTIL_BULLET_USER_PTR_INFO_HPP
#define RADIX_UTIL_BULLET_USER_PTR_INFO_HPP

#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>

#include <radix/entities/traits/Trait.hpp>
#include <radix/Entity.hpp>

namespace radix {
namespace util {

struct BulletUserPtrInfo {
  Entity *entity;
  entities::Trait *trait;

  inline BulletUserPtrInfo(Entity *e = nullptr, entities::Trait *t = nullptr) :
    entity(e),
    trait(t) {
  }
};

inline const BulletUserPtrInfo& getBtPtrInfo(const btCollisionObject *o) {
  return *reinterpret_cast<BulletUserPtrInfo*>(o->getUserPointer());
}

} /* namespace util */
} /* namespace radix */

#endif /* RADIX_UTIL_BULLET_USER_PTR_INFO_HPP */
