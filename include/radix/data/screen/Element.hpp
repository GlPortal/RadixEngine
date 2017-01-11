#ifndef RADIX_ELEMENT_HPP
#define RADIX_ELEMENT_HPP

#include <vector>
#include <radix/data/screen/Style.hpp>

namespace radix {
/**
 * Represents a screen element in the DOM
 */
class Element {
public:
  std::vector<Element> elements;
  std::vector<std::reference_wrapper<Style>> styleReferences;
  Style style;
  void computeStyle();
  Style getComputedStyle();
private:
  Style computedStyle;
};
} /* namespace radix */

#endif /* RADIX_ELEMENT_HPP */
