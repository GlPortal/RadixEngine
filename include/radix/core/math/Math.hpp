#ifndef MATH_HPP
#define MATH_HPP

#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Vector4f.hpp>

namespace radix {

/** \class Math
 * @brief Math helper class
 */
class Math {
public:
  static constexpr float PI = 3.14159265358979323846f;
  static constexpr float DEG_TO_RAD = PI / 180;
  static constexpr float RAD_TO_DEG = 180 / PI;

  static Vector3f toDirection(const Quaternion &orientation);
  static Vector3f toEuler(const Vector3f &direction);

  /** Restricts a value to a range
   * @param v    Value
   * @param low  Low bound
   * @param high High bound
   */
  template <typename T>
  static constexpr inline T clamp(T v, T low, T high) {
    if (v < low) {
      return low;
    } else if (v > high) {
      return high;
    }
    return v;
  }

  /** Returns the sign of value
   * @param v value
   * @returns  1 if v is positive
   *          -1 if v is negative
   *           0 if v is 0
   */
  template <typename T>
  static constexpr inline T sign(T v) {
    if (v > 0) {
      return 1;
    }
    if (v < 0) {
      return -1;
    }
    return 0;
  }
};

constexpr inline float deg(float rad) {
  return rad * Math::RAD_TO_DEG;
}

constexpr inline float rad(float deg) {
  return deg * Math::DEG_TO_RAD;
}

} /* namespace radix */

#endif /* MATH_HPP */
