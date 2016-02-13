#include <radix/physics/PhysicsHelper.hpp>

#include <cmath>

#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h>

namespace glPortal {

bool PhysicsHelper::pointInAABB(const btVector3 &p, const btVector3 &min, const btVector3 &max) {
  return p.x() >= min.x() && p.x() <= max.x() &&
         p.y() >= min.y() && p.y() <= max.y() &&
         p.z() >= min.z() && p.z() <= max.z();
}

bool PhysicsHelper::pointInVolume(const btVector3 &p, const btCollisionObject &o) {
  btVector3 min, max;
  const btCollisionShape &s = *o.getCollisionShape();
  const btTransform &t = o.getWorldTransform();
  s.getAabb(t, min, max);
  // So far the following technique only works on collision shapes inheriting
  // from btPolyhedralConvexShape. Check for those.
  if (pointInAABB(p, min, max) && s.isPolyhedral() && s.isConvex()) {
    // Stupid margins.
    const btPolyhedralConvexShape &cs = (const btPolyhedralConvexShape&)s;
    return cs.isInside(t.inverse()(p), cs.getMargin());
  }
  return false;
}

} /* namespace glPortal */
