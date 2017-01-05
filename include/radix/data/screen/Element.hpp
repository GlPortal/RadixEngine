#ifndef RADIX_ELEMENT_HPP
#define RADIX_ELEMENT_HPP

#include <vector>

#include <radix/core/math/Vector4f.hpp>

namespace radix {

class Element {
public:
  static const std::string POSITION_ABSOLUTE;
  static const std::string POSITION_RELATIVE;
  std::vector<Element> elements;
  Vector4f color;
  unsigned int zIndex;
  Vector4f position;
  Vector4f margin;
  Vector4f padding;
};
} /* namespace radix */

#endif /* RADIX_ELEMENT_HPP */
