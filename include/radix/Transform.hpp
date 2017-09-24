#ifndef RADIX_TRANSFORM_HPP
#define RADIX_TRANSFORM_HPP

#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Quaternion.hpp>
#include <radix/core/math/Matrix4f.hpp>

class btTransform;

namespace radix {

class Transform {
public:
  Vector3f position, scale{1};
  Quaternion orientation;

  void applyModelMtx(Matrix4f&) const;
  void getModelMtx(Matrix4f&) const;

  operator btTransform() const;
};

} /* namespace radix */

#endif /* RADIX_TRANSFORM_HPP */
