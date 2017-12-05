#ifndef VECTOR2I_HPP
#define VECTOR2I_HPP

#include <string>

namespace radix {

struct Vector2i {
  union {
    int x, width;
  };
  union {
    int y, height;
  };

  static const Vector2i ZERO, UP;

  constexpr Vector2i()
  : x(0), y(0) {}
  constexpr Vector2i(int x, int y)
  : x(x), y(y) {}
  constexpr Vector2i(int v)
  : x(v), y(v) {}

  std::string str() const;

  constexpr bool operator==(const Vector2i &v) const {
    return x == v.x && y == v.y;
  }

  constexpr bool operator!=(const Vector2i &v) const {
    return x != v.x || y != v.y;
  }

  constexpr Vector2i operator-(const Vector2i& v) const {
    return Vector2i(x - v.x, y - v.y);
  }
};
} /* namespace radix */
#endif /* VECTOR2I_HPP */
