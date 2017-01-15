#include <radix/data/screen/Element.hpp>

namespace radix {

Element::Element():needsRecomputation(true) {
}

void Element::addStyle(Style& style) {
  needsRecomputation = true;
  styles.push_back(style);
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
