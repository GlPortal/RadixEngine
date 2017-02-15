#ifndef RADIX_ARGUMENTS_PARSER_HPP
#define RADIX_ARGUMENTS_PARSER_HPP

#include <string>
#include <radix/env/Config.hpp>

namespace radix {

class ArgumentsParser {
private:
  static std::string mapName;
  static std::string mapPath;
  static bool showCursor;
  static bool ignoreGlVersion;
  static bool debugMode;
public:
  static void showUsage(char **argv);
  static void setEnvironmentFromArgs(const int argc, char **argv);
  static void populateConfig(radix::Config &config);
  static void showVersion();
};

} /* namespace radix */

#endif /* RADIX_ARGUMENTS_PARSER_HPP */
