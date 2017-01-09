#ifndef RADIX_ELEMENT_HPP
#define RADIX_ELEMENT_HPP

#include <vector>

#include <radix/core/math/Vector4f.hpp>

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
  Vector3f position;
  Vector4f margin;
  Vector4f padding;
  AlignMode alignMode;
  PositionMode positionMode;
};
} /* namespace radix */

#endif /* RADIX_ELEMENT_HPP */
