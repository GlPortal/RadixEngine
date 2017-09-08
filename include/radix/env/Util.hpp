#ifndef RADIX_UTIL_HPP
#define RADIX_UTIL_HPP

#include <string>
#include <memory>
#include <random>
#include <thread>

#include <radix/core/diag/LogInput.hpp>

namespace radix {

class Util {
private:
  static std::unique_ptr<Logger> logger;

public:
/*! \cond PRIVATE */
  struct _Log {
    inline LogInput operator()(LogLevel lvl) {
      return LogInput(*Util::logger, lvl);
    }
    inline LogInput operator()(LogLevel lvl, const std::string &tag) {
      return LogInput(*Util::logger, lvl, tag);
    }
    inline LogInput operator()(LogLevel lvl, const char *tag) {
      return LogInput(*Util::logger, lvl, tag);
    }
  };
/*! \endcond */
  static _Log Log;

  static std::random_device RandDev;
  static std::mt19937 Rand;

  static void Init();

  static void SetThreadName(std::thread&, const char*);
  inline static void SetThreadName(std::thread &thread, const std::string &name) {
    SetThreadName(thread, name.c_str());
  }
};

} /* namespace radix */

#endif /* RADIX_UTIL_HPP */
