#ifndef RADIX_TEXT_HPP
#define RADIX_TEXT_HPP

#include <string>

#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Vector4f.hpp>

namespace radix {

struct Text {
  enum Align {
    Left,
    Right,
    Center
  };

  Vector4f color;
  Vector3f position;
  std::string id; //for styling and scripting
  std::string type; //for styling
  std::string font;
  std::string content;
  Align align;
  float z;
  float top;
  float size;
};

} /* namespace radix */

#endif /* RADIX_TEXT_HPP */
