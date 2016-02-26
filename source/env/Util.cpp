#include <radix/env/Util.hpp>

#include <SDL2/SDL_platform.h>

#include <radix/core/diag/AnsiConsoleLogger.hpp>
#include <radix/core/diag/StdoutLogger.hpp>

namespace radix {

std::unique_ptr<Logger> Util::logger;
Util::_Log Util::Log;

std::random_device Util::RandDev;
std::mt19937 Util::Rand(Util::RandDev());

void Util::Init() {
  if (std::string("Linux") == SDL_GetPlatform()) {
    logger.reset(new AnsiConsoleLogger);
  } else {
    logger.reset(new StdoutLogger);
  }
}

LogInput Util::_Log::operator()() {
  return LogInput(*Util::logger, LogLevel::Debug);
}

LogInput Util::_Log::operator()(LogLevel lvl) {
  return LogInput(*Util::logger, lvl);
}

LogInput Util::_Log::operator()(LogLevel lvl, const std::string &tag) {
  return LogInput(*Util::logger, lvl, tag);
}

} /* namespace radix */
