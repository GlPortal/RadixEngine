#ifndef RADIX_CONSOLE_HPP
#define RADIX_CONSOLE_HPP

#include <radix/core/diag/Logger.hpp>

namespace radix {

class Console : public Logger {
public:
  void render();
};

} /* namespace radix */
#endif /* RADIX_CONSOLE_HPP */