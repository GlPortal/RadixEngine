#ifndef RADIX_SCREEN_HPP
#define RADIX_SCREEN_HPP

#include <string>
#include <vector>
#include <radix/core/math/Vector4f.hpp>

namespace radix {

struct Text {
  std::string text;
  int x, y;
  float size;
};

struct Screen {

  std::vector<Text> text;
  float alpha;
  Vector4f bgColor;
  Vector4f textColor;
};
} /* namespace radix */

#endif //RADIX_SCREEN_HPP
