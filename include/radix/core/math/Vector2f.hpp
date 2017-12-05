/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Vector2f.hpp
** Declares a vector consisting of 2 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#pragma once
#ifndef VECTOR2F_HPP
#define VECTOR2F_HPP

#include <radix/core/math/Vector2i.hpp>

#include <string>

namespace radix {

/** \class Vector2f
 * @brief 2-dimensional `float`-based vector/point storage and manipulation struct
 */
struct Vector2f {
  union {
    float x, r, s, u;
  };
  union {
    float y, g, t, v;
  };

  static const Vector2f ZERO, UP;

  /* Core */
  constexpr Vector2f()
    : x(0), y(0) {}
  constexpr Vector2f(float x, float y)
    : x(x), y(y) {}
  constexpr Vector2f(float v)
    : x(v), y(v) {}

  constexpr Vector2f(const Vector2i &v)
    : x(v.x), y(v.y) {}

  float length() const;
  std::string str() const;

  /* Operator overloads */
  constexpr bool operator==(const Vector2f &v) const {
    return x == v.x && y == v.y;
  }

  constexpr bool operator!=(const Vector2f &v) const {
    return x != v.x || y != v.y;
  }

  constexpr Vector2f operator-() const {
    return Vector2f(-x, -y);
  }

  constexpr Vector2f operator+(const Vector2f& v) const {
    return Vector2f(x + v.x, y + v.y);
  }
  Vector2f& operator+=(const Vector2f &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  constexpr Vector2f operator-(const Vector2f& v) const {
    return Vector2f(x - v.x, y - v.y);
  }
  Vector2f& operator-=(const Vector2f& v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  constexpr Vector2f operator*(const Vector2f& v) const {
    return Vector2f(x * v.x, y * v.y);
  }
  Vector2f& operator*=(const Vector2f& v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  constexpr Vector2f operator*(float scale) const {
    return Vector2f(x * scale, y * scale);
  }
  Vector2f& operator*=(float scale) {
    x *= scale;
    y *= scale;
    return *this;
  }

  constexpr Vector2f operator/(float divisor) const {
    return Vector2f(x / divisor, y / divisor);
  }
  Vector2f& operator/=(float divisor) {
    x /= divisor;
    y /= divisor;
    return *this;
  }

  constexpr Vector2f operator/(const Vector2f& v) const {
    return Vector2f(x / v.x, y / v.y);
  }
  constexpr Vector2f operator/=(const Vector2f& v) const {
    return Vector2f(x / v.x, y / v.y);
  }
  Vector2f& operator/=(const Vector2f& v) {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  bool fuzzyEqual(const Vector2f&, float threshold = .02f) const;
};

/* Utility functions */
constexpr inline float dot(const Vector2f &v1, const Vector2f &v2) {
  return v1.x * v2.x + v1.y * v2.y;
}
inline Vector2f normalize(const Vector2f &v) {
  return v / v.length();
}

} /* namespace radix */

#endif /* VECTOR2F_HPP */
