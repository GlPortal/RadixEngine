#ifndef RADIX_SCREEN_HPP
#define RADIX_SCREEN_HPP

#include <vector>

#include <radix/screen/text/Text.hpp>
#include <radix/core/math/Vector4f.hpp>

namespace radix {

struct Screen {
  Vector4f color;
  std::vector<Text> text;
};

} /* namespace radix */

#endif /* RADIX_SCREEN_HPP */
