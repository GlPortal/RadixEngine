#include <radix/env/ArgumentsParser.hpp>

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <getopt.h>
#include <iostream>

#include <radix/core/file/Path.hpp>
#include <radix/env/Config.hpp>
#include <radix/env/Environment.hpp>

namespace radix {
const int ArgumentsParser::NO_ARGUMENT = -1;
std::string ArgumentsParser::mapName = "";
std::string ArgumentsParser::mapPath = "";
bool ArgumentsParser::showCursor = false;
bool ArgumentsParser::ignoreGlVersion = false;
bool ArgumentsParser::debugMode = false;
bool ArgumentsParser::consoleEnabled = false;

void ArgumentsParser::showUsage(char **argv) {
  std::cout << "Usage: " << argv[0]  << " [options]" << std::endl << std::endl;

  std::cout << "Options:" << std::endl;
  std::cout << "  -h, --help               Show this help message and exit" << std::endl;
  std::cout << "  -v, --version            Display GlPortal version" << std::endl;
  std::cout << "  -d, --datadir DIR        Set the data directory" << std::endl;
  std::cout << "  -m, --map NAME           Specify map name to load" << std::endl;
  std::cout << "  -M, --mapfrompath FILE   Load the specified map file" << std::endl;
  std::cout << "  -c, --showcursor         Forces to draw os mouse cursor" << std::endl;
  std::cout << "  -G, --ignoreGlVersion    Disable OpenGl version check" << std::endl;
  std::cout << "  -D, --debugMode          Run game in debug mode" << std::endl;
  std::cout << "  -a, --console            Run game interactively" << std::endl;
}

void ArgumentsParser::setEnvironmentFromArgs(const int argc, char **argv) {
  static struct option longOptions[] = {
    {"version",          no_argument,       0, 'v'},
    {"help",             no_argument,       0, 'h'},
    {"showcursor",       no_argument,       0, 'c'},
    {"ignoreGlVersion",  no_argument,       0, 'G'},
    {"debugMode",        no_argument,       0, 'D'},
    {"datadir",          required_argument, 0, 'd'},
    {"map",              required_argument, 0, 'm'},
    {"mapfrompath",      required_argument, 0, 'M'},
    {"console",          no_argument      , 0, 'a'},
    {0, 0, 0, 0}
  };

  while (1) {
    int optionIndex = 0;
    int argument;
    argument = getopt_long(argc, argv, "cvhad:m:M:GD", longOptions, &optionIndex);

    if (argument == NO_ARGUMENT) {
      break;
    }

    switch (argument) {
    case 'v':
      showVersion();
      exit(0);
    case 'd':
      Environment::setDataDir(optarg);
      break;
    case 'h':
      showUsage(argv);
      exit(0);
    case 'm':
      mapName = optarg;
      break;
    case 'M':
      mapPath = optarg;
      break;
    case 'c':
      showCursor = true;
      break;
    case '?':
      showUsage(argv);
      exit(1);
    case 'G':
      ignoreGlVersion = true;
      break;
    case 'D':
      debugMode = true;
      showCursor = true;
      ignoreGlVersion = true;
      break;
    case 'a':
      consoleEnabled = true;
      break;
    default:
      break;
    }
  }
}

void ArgumentsParser::populateConfig(radix::Config &config) {
  if (not mapName.empty()) {
    config.map = mapName;
  }

  if (not mapPath.empty()) {
    config.mapPath = mapPath;
  }

  if (showCursor) {
    config.cursorVisibility = showCursor;
  }

  if (ignoreGlVersion) {
    config.ignoreGlVersion = ignoreGlVersion;
  }

  if (debugMode) {
    config.width = 800;
    config.height = 600;
    config.fullscreen = false;
  }

  if (consoleEnabled) {
    config.consoleEnabled = true;
  }
}

void ArgumentsParser::showVersion(){
  std::cout << "GlPortal Version 0.1\n";
}

} /* namespace radix */
