#include <radix/data/screen/Element.hpp>

namespace radix {

void Element::computeStyle() {
}

Style Element::getStyle() {
  computeStyle();
  return computedStyle;
}

} /* namespace radix */
