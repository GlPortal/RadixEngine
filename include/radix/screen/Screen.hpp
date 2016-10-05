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
  float alpha = 0;
  /* Color is stored in RGBA format
   * Thus x= r, y = g, etc.. */
  Vector4f bgColor;
  Vector4f textColor;
};
} /* namespace radix */

#endif //RADIX_SCREEN_HPP
