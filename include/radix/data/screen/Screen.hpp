#ifndef RADIX_SCREEN_HPP
#define RADIX_SCREEN_HPP

#include <vector>

#include <radix/data/text/Text.hpp>
#include <radix/data/screen/Element.hpp>
#include <radix/core/math/Vector4f.hpp>

namespace radix {

struct Screen {
  Vector4f color;
  std::vector<Text> text;
  std::vector<Element> elements;
};

} /* namespace radix */

#endif /* RADIX_SCREEN_HPP */
