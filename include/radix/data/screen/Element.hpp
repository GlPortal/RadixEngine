#ifndef RADIX_ELEMENT_HPP
#define RADIX_ELEMENT_HPP

#include <vector>

#include <radix/core/math/Vector4f.hpp>
#include <radix/core/math/Vector4i.hpp>

namespace radix {
enum PositionMode {
  absolute,
  relative
};

enum AlignMode {
  left,
  right,
  center
};

/**
 * Represents a screen element in the DOM
 */
struct Element {
  std::vector<Element> elements;
  Vector4f color;
  Vector4i position;
  Vector4i margin;
  Vector4i padding;
  AlignMode alignMode;
  PositionMode positionMode;
};
} /* namespace radix */

#endif /* RADIX_ELEMENT_HPP */
