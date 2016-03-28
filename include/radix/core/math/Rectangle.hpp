#pragma once
#ifndef RADIX_RECTANGLE_HPP
#define RADIX_RECTANGLE_HPP

namespace radix {

/** \class Rectangle
 */
template<typename T>
struct Rectangle {
  T x, y, w, h;

  /* Core */
  constexpr Rectangle() {}
  constexpr Rectangle(T x, T y, T w, T h) :
    x(x), y(y), w(w), h(h) {}

  constexpr void set(T x, T y, T w, T h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
  }

  constexpr bool hasZeroArea() const {
    return w == 0 and h == 0;
  }

  /* Operator overloads */
  constexpr bool operator==(const Rectangle<T> &o) const {
    return x == o.x and
           y == o.y and
           w == o.w and
           h == o.h;
  }
  constexpr bool operator!=(const Rectangle<T> &o) const {
    return !operator==(o);
  }
};

using RectangleI = Rectangle<int>;
using RectangleF = Rectangle<float>;
using RectangleD = Rectangle<double>;

} /* namespace radix */

#endif /* RADIX_RECTANGLE_HPP */
