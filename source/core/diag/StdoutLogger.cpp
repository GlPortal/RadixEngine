#include <radix/core/diag/StdoutLogger.hpp>

#include <iostream>

#ifdef _WIN32
#include <ciso646>
#endif

namespace radix {

const char* StdoutLogger::getName() const {
  return "stdout logger";
}

void StdoutLogger::log(const std::string &message, LogLevel lvl, const std::string &tag) {
  std::unique_lock<std::mutex> lk(mtx);

  if (not tag.empty()) {
    std::cout << tag << ' ';
  }
  std::cout << message << std::endl;
}

} /* namespace radix */
