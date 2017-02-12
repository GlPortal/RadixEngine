#include <radix/core/math/Math.hpp>

#include <cmath>

#include <radix/core/math/Matrix4f.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Quaternion.hpp>

namespace radix {
Vector3f Math::toDirection(const Quaternion &orientation) {
  return orientation * Vector3f::FORWARD;
}

Vector3f Math::toEuler(const Vector3f &direction) {
  Vector3f euler;

  // Pitch
  euler.x = asin(direction.y);

  // Yaw
  if (direction.x <= 0 && direction.z < 0) {
    euler.y = atan(fabs(direction.x) / fabs(direction.z));
  }
  if (direction.x < 0 && direction.z >= 0) {
    euler.y = atan(fabs(direction.z) / fabs(direction.x)) + rad(90);
  }
  if (direction.x >= 0 && direction.z > 0) {
    euler.y = atan(fabs(direction.x) / fabs(direction.z)) + rad(180);
  }
  if (direction.x > 0 && direction.z <= 0) {
    euler.y = atan(fabs(direction.z) / fabs(direction.x)) + rad(270);
  }

  return euler;
}

float Math::randFloatRange(float min, float max) {
  srand(static_cast<unsigned> (time(0)));
  float random = ((float) rand()) / (float) RAND_MAX;
  float difference = max - min;
  float result = random * difference;
  return min + result;
}
/* implementation of glm::lookat */
Matrix4f Math::lookAt(const Vector3f &eye, const Vector3f &center, const Vector3f &up) {
  Matrix4f matrix;
  Vector3f x, y, z;
  z = eye - center;
  normalize(z);
  y = up;
  x = cross(y, z);
  y = cross(z, x);
  normalize(x);
  normalize(y);
  matrix[0] = x.x;
  matrix[1] = x.y;
  matrix[2] = x.z;
  matrix[3] = -dot(x, eye);
  matrix[4] = y.x;
  matrix[5] = y.y;
  matrix[6] = y.z;
  matrix[7] = -dot(y, eye);
  matrix[8] = z.x;
  matrix[9] = z.y;
  matrix[10] = z.z;
  matrix[11] = -dot(z, eye);
  matrix[12] = 0;
  matrix[13] = 0;
  matrix[14] = 0;
  matrix[15] = 0;

  return matrix;
}

} /* namespace radix */
