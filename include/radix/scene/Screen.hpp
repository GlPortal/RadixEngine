#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <string>

#include <radix/core/math/Vector4f.hpp>

namespace radix {

class Screen {
public:
  Screen(std::string title, std::string text);
  std::string title;
  std::string text;
  bool enabled;
  float alpha;
  Vector4f backgroundColor;
  Vector4f textColor;
};

} /* namespace radix */

#endif /* SCREEN_HPP */
