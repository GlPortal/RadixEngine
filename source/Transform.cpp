#include <radix/Transform.hpp>

#include <bullet/LinearMath/btTransform.h>

namespace radix {

/*void Transform::serialize(serine::Archiver &ar) {
  ar("pos", position);
  if (ar.isLoading() || (ar.isSaving() && (scale.x != 0 || scale.y != 0 || scale.z != 0))) {
    ar("scl", scale);
  }
  ar("orient", orientation);
}*/

void Transform::applyModelMtx(Matrix4f &m) const {
  m.translate(position);
  m.rotate(orientation);
  m.scale(scale);
}

void Transform::getModelMtx(Matrix4f &m) const {
  m.setIdentity();
  applyModelMtx(m);
}

Transform::operator btTransform() const {
  return btTransform(orientation, position);
}

} /* namespace radix */
