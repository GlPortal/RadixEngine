#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <string>

#include <radix/core/math/Vector4f.hpp>

namespace radix {

class Terminal {
public:
  Terminal();
  bool enabled;
  Vector4f backgroundColor;
  Vector4f textColor;
};

} /* namespace radix */

#endif /* TERMINAL_HPP */
