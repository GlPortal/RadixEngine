#include <radix/core/diag/StdoutLogger.hpp>

#include <iostream>

namespace radix {

const char* StdoutLogger::getName() const {
  return "stdout logger";
}

void StdoutLogger::log(const std::string &message, LogLevel lvl, const std::string &tag) {
  std::unique_lock<std::mutex> lk(mtx);

  if ( !tag.empty()) {
    std::cout << tag << ' ';
  }
  std::cout << message << std::endl;
}

} /* namespace radix */
