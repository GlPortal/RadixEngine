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
  Style style;
  Style getStyle(); 
  std::vector<Element> getSubElements();
private:
  bool changeFlag;
  std::vector<Element> elements;
  std::vector<std::reference_wrapper<Style>> styleReferences;
  void computeStyle();
  Style computedStyle;
};
} /* namespace radix */

#endif /* RADIX_ELEMENT_HPP */
