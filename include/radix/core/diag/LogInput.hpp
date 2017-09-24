#ifndef RADIX_LOGINPUT_HPP
#define RADIX_LOGINPUT_HPP

#include <string>
#include <type_traits>

#include <stx/string_view.hpp>

#include <radix/core/math/Vector2f.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Vector4f.hpp>
#include <radix/core/diag/Logger.hpp>

namespace radix {

/** \class LogInput
 * @brief `std::cout`-like object that sends the data it aggregated to a Logger when it dies
 */
class LogInput {
protected:
  Logger &sink;
  LogLevel lvl;
  std::string buf, tag;

  template <typename N>
  inline LogInput& appendNumber(N number) {
    buf.append(std::to_string(number));
    return *this;
  }

public:
  LogInput(Logger &sink, LogLevel lvl, const char *tag = "");
  LogInput(Logger &sink, LogLevel lvl, const std::string &tag);
  ~LogInput();

  LogInput(const LogInput&) = delete;
  LogInput& operator=(const LogInput&) = delete;

  LogInput(LogInput&&);
  LogInput& operator=(LogInput&&) = delete;

  LogInput& operator<<(const char*);
  LogInput& operator<<(const std::string&);
  LogInput& operator<<(const stx::string_view&);

  LogInput& operator<<(bool);
  LogInput& operator<<(char);

  // std::to_string overloads
  LogInput& operator<<(int v) { return appendNumber(v); }
  LogInput& operator<<(long v) { return appendNumber(v); }
  LogInput& operator<<(long long v) { return appendNumber(v); }
  LogInput& operator<<(unsigned v) { return appendNumber(v); }
  LogInput& operator<<(unsigned long v) { return appendNumber(v); }
  LogInput& operator<<(unsigned long long v) { return appendNumber(v); }
  LogInput& operator<<(float v) { return appendNumber(v); }
  LogInput& operator<<(double v) { return appendNumber(v); }
  LogInput& operator<<(long double v) { return appendNumber(v); }

  // std::to_string-compatible upcasts
  LogInput& operator<<(unsigned char v) { return appendNumber<unsigned>(v); }
  LogInput& operator<<(short v) { return appendNumber<int>(v); }
  LogInput& operator<<(unsigned short v) { return appendNumber<unsigned>(v); }

  LogInput& operator<<(const void*);

  LogInput& operator<<(const Vector2f&);
  LogInput& operator<<(const Vector3f&);
  LogInput& operator<<(const Vector4f&);
};

} /* namespace radix */

#endif /* RADIX_LOGINPUT_HPP */
