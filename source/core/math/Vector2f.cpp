/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Vector2f.cpp
** Implements a vector consisting of 2 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#include <radix/core/math/Vector2f.hpp>

#include <cmath>
#include <sstream>

#include <ciso646>

namespace radix {

const Vector2f Vector2f::ZERO(0, 0);
const Vector2f Vector2f::UP(0, 1);

/* Core */
float Vector2f::length() const {
  return sqrt(x * x + y * y);
}

std::string Vector2f::str() const {
  std::stringstream ss;
  ss << "(" << x << ", " << y << ")";
  return ss.str();
}

bool Vector2f::fuzzyEqual(const Vector2f &v, float threshold) const {
  return (x > v.x - threshold and x < v.x + threshold) and
         (y > v.y - threshold and y < v.y + threshold);
}

} /* namespace radix */
