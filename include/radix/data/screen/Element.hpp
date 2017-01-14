#ifndef RADIX_ELEMENT_HPP
#define RADIX_ELEMENT_HPP

#include <memory>
#include <vector>
#include <radix/data/screen/Style.hpp>

namespace radix {
/**
 * Represents a screen element in the DOM
 */
class Element {
public:
  Element();
  Style style;
  Style getStyle();
  std::vector<Element> getSubElements();
private:
  bool needsRecomputation;
  std::vector<Element> elements;
  std::vector<std::reference_wrapper<Style>> styles;
  void computeStyle();
  Style computedStyle;
};
} /* namespace radix */

#endif /* RADIX_ELEMENT_HPP */
