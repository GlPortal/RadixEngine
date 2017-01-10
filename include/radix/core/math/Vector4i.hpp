#ifndef VECTOR4I_HPP
#define VECTOR4I_HPP
struct Vector4i {
  union {
    int x, top, r;
  };
  union {
    int y, bottom, g;
  };
  union {
    int z, left, b;
  };
  union {
    int w, right, a;
  };

  constexpr Vector4i()
  : x(0), y(0), z(0), w(0) {}
  constexpr Vector4i(int x, int y, int z, int w)
  : x(x), y(y), z(z), w(w) {}
  constexpr Vector4i(int v)
  : x(v), y(v), z(v), w(v) {}

};
#endif /* VECTOR4I_HPP */
