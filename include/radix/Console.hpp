#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <radix/core/diag/Logger.hpp>

namespace radix {

class Console : public Logger {
public:
  void render();
};

} /* namespace radix */
#endif /* CONSOLE_HPP */