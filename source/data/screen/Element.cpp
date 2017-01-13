#include <radix/data/screen/Element.hpp>

namespace radix {

Element::Element():needsRecomputation(true) {
}

void Element::computeStyle() {
}

Style Element::getStyle() {
  if (needsRecomputation) {
    computeStyle();
  }

  return computedStyle;
}

} /* namespace radix */
