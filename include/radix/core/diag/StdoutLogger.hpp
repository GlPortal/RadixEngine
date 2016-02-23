#ifndef STDOUT_LOGGER_HPP
#define STDOUT_LOGGER_HPP

#include <string>

#include <radix/core/diag/Logger.hpp>

namespace radix {

/** \class StdoutLogger
 * @brief Logger that outputs to an ANSI/vt-100 console
 */
class StdoutLogger : public Logger {
public:
  const char* getName() const;
  void log(const std::string &message, LogLevel lvl, const std::string &tag);
};

} /* namespace radix */

#endif /* STDOUT_LOGGER_HPP */
