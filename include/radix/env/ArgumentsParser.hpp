#ifndef RADIX_ARGUMENTS_PARSER_HPP
#define RADIX_ARGUMENTS_PARSER_HPP

#include <string>

namespace radix {

class ArgumentsParser {
private:
  static std::string mapName;
  static std::string mapPath;
  static bool showCursor;
public:
  static void showUsage(char **argv);
  static void setEnvironmentFromArgs(const int argc, char **argv);
  static void populateConfig();
};

} /* namespace radix */

#endif /* RADIX_ARGUMENTS_PARSER_HPP */
