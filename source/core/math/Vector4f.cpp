#include <radix/core/math/Vector4f.hpp>

#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/LinearMath/btVector3.h>

#include <serine/Archiver.hpp>

#include <radix/core/math/Vector2f.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Matrix4f.hpp>
#include <radix/core/math/Math.hpp>

namespace radix {

Vector4f::Vector4f(const Vector3f &v, float w)
  : x(v.x), y(v.y), z(v.z), w(w) {}

Vector4f::Vector4f(const Vector2f &v, float z, float w)
  : x(v.x), y(v.y), z(z), w(w) {}

Vector4f::operator btVector4() const {
  return btVector4(x, y, z, w);
}

Vector4f::Vector4f(const btVector4 &v)
  : x(v.x()), y(v.y()), z(v.z()), w(v.w()) {}

Vector4f& Vector4f::operator=(const btVector4 &v) {
  x = v.x(); y = v.y(); z = v.z(); w = v.w();
  return *this;
}

bool Vector4f::fuzzyEqual(const Vector4f &v, float threshold) const {
  return (x > v.x - threshold && x < v.x + threshold) &&
         (y > v.y - threshold && y < v.y + threshold) &&
         (z > v.z - threshold && z < v.z + threshold) &&
         (w > v.w - threshold && w < v.w + threshold);
}

Vector4f::operator btQuaternion() const {
  return btQuaternion(x, y, z, w);
}

Vector4f::Vector4f(const btQuaternion &q)
  : x(q.x()), y(q.y()), z(q.z()), w(q.w()) {}

Vector4f& Vector4f::operator=(const btQuaternion &q) {
  x = q.x(); y = q.y(); z = q.z(); w = q.w();
  return *this;
}

void Vector4f::serialize(serine::Archiver &ar) {
  ar("x", x);
  ar("y", y);
  ar("z", z);
  ar("w", w);
}

// === Quaternion === //

Quaternion::Quaternion(const btQuaternion &v)
  : Vector4f(v.x(), v.y(), v.z(), v.w()) {}

Quaternion Quaternion::operator*(const Quaternion &q) const {
  return Quaternion(
    w * q.x + x * q.w + y * q.z - z * q.y,
    w * q.y + y * q.w + z * q.x - x * q.z,
    w * q.z + z * q.w + x * q.y - y * q.x,
    w * q.w - x * q.x - y * q.y - z * q.z
  );
}

Quaternion& Quaternion::operator*=(const Quaternion &q) {
  x = w * x + x * w + y * z - z * y;
  y = w * y + y * w + z * x - x * z;
  z = w * z + z * w + x * y - y * x;
  w = w * w - x * x - y * y - z * z;
  return *this;
}

Vector3f Quaternion::operator*(const Vector3f &v) const {
  // Pout = q * Pin * conj(q)
  Vector3f vn = normalize(v);
  Quaternion resQuat = *this * Quaternion(vn.x, vn.y, vn.z, 0) * conjugate(*this);
  return Vector3f(resQuat.x, resQuat.y, resQuat.z);
}


Quaternion& Quaternion::fromAxAngle(const Vector3f &a, float r) {
  Vector3f na = normalize(a);
  const float r2 = r / 2, sinR2 = std::sin(r2);
  this->x = na.x * sinR2;
  this->y = na.y * sinR2;
  this->z = na.z * sinR2;
  this->w = std::cos(r2);
  return *this;
}

Quaternion& Quaternion::fromAxAngle(float x, float y, float z, float r) {
  return fromAxAngle(Vector3f(x, y, z), r);
}

Quaternion& Quaternion::fromAxAngle(const Vector4f &a) {
  return fromAxAngle(a.x, a.y, a.z, a.w);
}

Vector4f Quaternion::toAxAngle() const {
  const float r = std::acos(w)*2, invSinR2 = 1/std::sin(r/2);
  return Vector4f(
    x * invSinR2,
    y * invSinR2,
    z * invSinR2,
    r
  );
}


Quaternion& Quaternion::fromAero(float tetha, float phi, float psi) {
  /*
   * thetha => rotation around Y
   * phi => rotation around X
   * psi => rotation around Z
   *
   * The convention used in term of rotation order is
   * RyRxRz
   */
  const float hY = tetha * 0.5, hP = phi * 0.5, hR = psi * 0.5;
  const float cY = std::cos(hY);
  const float sY = std::sin(hY);
  const float cP = std::cos(hP);
  const float sP = std::sin(hP);
  const float cR = std::cos(hR);
  const float sR = std::sin(hR);
  const float cRcP = cR*cP, cRsP = cR*sP, sRcP = sR*cP, sRsP = sR*sP;
  x = cRsP * cY + sRcP * sY;
  y = cRcP * sY - sRsP * cY;
  z = sRcP * cY - cRsP * sY;
  w = cRcP * cY + sRsP * sY;

  return *this;
}

Quaternion& Quaternion::fromAero(const Vector3f &v) {
  fromAero(v.tetha, v.phi, v.psi);
  return *this;
}

Vector3f Quaternion::toAero() const {
  // http://www.geometrictools.com/Documentation/EulerAngles.pdf

  //extract each column of the rotation matrix
  const Vector3f firstColumn = (*this) * Vector3f{1,0,0};
  const Vector3f secondColumn = (*this) * Vector3f{0,1,0};
  const Vector3f thirdColumn = (*this) * Vector3f{0,0,1};

  //extract each coefficient from the rotation matrice
  const float r00 = firstColumn.x;
  const float r10 = firstColumn.y;
  const float r20 = firstColumn.z;

  const float r01 = secondColumn.x;
  const float r11 = secondColumn.y;
  const float r21 = secondColumn.z;

  const float r02 = thirdColumn.x;
  const float r12 = thirdColumn.y;
  const float r22 = thirdColumn.z;

  float thetaX, thetaY, thetaZ;
  /*
   * Due to some numerical error a threeshold is needed.
   *
   * When an angle is near 90 degree some comparisons can 
   * fail even though they shouldn't. 
   * To avoid that we approximate any angle near 90 to be 90.
   */
  const float eps = 0.00001f;

  if (r12 < 1 - eps) {
    if (r12 > -1 + eps ) {
      thetaX = std::asin(-r12);
      thetaY = std::atan2(r02, r22);
      thetaZ = std::atan2(r10, r11);
    } else {
      thetaX = Math::PI /2;
      thetaY = -std::atan2(-r01, r00);
      thetaZ = 0;
    }
  } else {
    thetaX = -Math::PI/2;
    thetaY = std::atan2(-r01, r00);
    thetaZ = 0;
  }

  return Vector3f(thetaY, thetaX, thetaZ);
}

Matrix4f Quaternion::toMatrix() const {
  Matrix4f m;
  m[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
  m[1] = 2.0f*x*y + 2.0f*z*w;
  m[2] = 2.0f*x*z - 2.0f*y*w;
  m[4] = 2.0f*x*y - 2.0f*z*w;
  m[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
  m[6] = 2.0f*y*z + 2.0f*x*w;
  m[8] = 2.0f*x*z + 2.0f*y*w;
  m[9] = 2.0f*y*z - 2.0f*x*w;
  m[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
  return m;
}

} /* namespace radix */
