#ifndef RADIX_LOGGER_HPP
#define RADIX_LOGGER_HPP

#include <string>

namespace radix {

enum LogLevel {
  Verbose,
  Debug,
  Info,
  Warning,
  Error,
  Failure
};

/** \class Logger
 * @brief Base class to create log sinks
 */
class Logger {
public:
  /**
   * @brief Returns the name of the Logger instance
   */
  virtual const char* getName() const = 0;

  /**
   * @brief Log a message
   *
   * @param message Message to log
   * @param lvl     Log level
   * @param tag     Log tag, identifying the log category / class emitting the message
   */
  virtual void log(const std::string &message, LogLevel lvl, const std::string &tag) = 0;
};

} /* namespace radix */

#endif /* RADIX_LOGGER_HPP */
