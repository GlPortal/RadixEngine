#ifndef VECTOR2I_HPP
#define VECTOR2I_HPP
struct Vector2i {
  union {
    int x, width;
  };
  union {
    int y, height;
  };

  constexpr Vector2i()
  : x(0), y(0) {}
  constexpr Vector2i(int x, int y)
  : x(x), y(y) {}
  constexpr Vector2i(int v)
  : x(v), y(v) {}

};
#endif /* VECTOR2I_HPP */
