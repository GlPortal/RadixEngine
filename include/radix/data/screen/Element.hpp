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

struct Element {
  std::vector<Element> elements;
  Vector4f color;
  unsigned int zIndex;
  Vector4f position;
  Vector4f margin;
  Vector4f padding;

};
} /* namespace radix */

#endif /* RADIX_ELEMENT_HPP */
