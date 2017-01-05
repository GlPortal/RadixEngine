#ifndef RADIX_ELEMENT_HPP
#define RADIX_ELEMENT_HPP

#include <vector>

#include <radix/core/math/Vector4f.hpp>

namespace radix {

  struct Element {
    std::vector<Element> elements;
    Vector4f color;
  };

} /* namespace radix */

#endif /* RADIX_ELEMENT_HPP */
