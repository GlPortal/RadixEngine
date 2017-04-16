#pragma once
#ifndef VECTOR2UI_HPP
#define VECTOR2UI_HPP

#include <string>

namespace radix {

/** \class Vector2ui
 * @brief 2-dimensional `unsigned int`-based storage and manipulation struct
 */
struct Vector2ui {
  union {
    unsigned int x, r, s, u;
  };
  union {
    unsigned int y, g, t, v;
  };

  static const Vector2ui ZERO, UP;

  /* Core */
  constexpr Vector2ui()
    : x(0), y(0) {}
  constexpr Vector2ui(unsigned int x, unsigned int y)
    : x(x), y(y) {}
  constexpr Vector2ui(unsigned int v)
    : x(v), y(v) {}

  unsigned int length() const;
  std::string str() const;

  /* Operator overloads */
  constexpr bool operator==(const Vector2ui &v) const {
    return x == v.x && y == v.y;
  }

  constexpr bool operator!=(const Vector2ui &v) const {
    return x != v.x || y != v.y;
  }

  constexpr Vector2ui operator-() const {
    return Vector2ui(-x, -y);
  }

  constexpr Vector2ui operator+(const Vector2ui& v) const {
    return Vector2ui(x + v.x, y + v.y);
  }
  Vector2ui& operator+=(const Vector2ui &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  constexpr Vector2ui operator-(const Vector2ui& v) const {
    return Vector2ui(x - v.x, y - v.y);
  }
  Vector2ui& operator-=(const Vector2ui& v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  constexpr Vector2ui operator*(const Vector2ui& v) const {
    return Vector2ui(x * v.x, y * v.y);
  }
  Vector2ui& operator*=(const Vector2ui& v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  constexpr Vector2ui operator*(unsigned int scale) const {
    return Vector2ui(x * scale, y * scale);
  }
  Vector2ui& operator*=(unsigned int scale) {
    x *= scale;
    y *= scale;
    return *this;
  }

  constexpr Vector2ui operator/(unsigned int divisor) const {
    return Vector2ui(x / divisor, y / divisor);
  }
  Vector2ui& operator/=(unsigned int divisor) {
    x /= divisor;
    y /= divisor;
    return *this;
  }

  constexpr Vector2ui operator/=(const Vector2ui& v) const {
    return Vector2ui(x / v.x, y / v.y);
  }
  Vector2ui& operator/=(const Vector2ui& v) {
    x /= v.x;
    y /= v.y;
    return *this;
  }
};

/* Utility functions */
constexpr inline unsigned int dot(const Vector2ui &v1, const Vector2ui &v2) {
  return v1.x * v2.x + v1.y * v2.y;
}
inline Vector2ui normalize(const Vector2ui &v) {
  return v / v.length();
}

} /* namespace radix */

#endif /* VECTOR2F_HPP */
