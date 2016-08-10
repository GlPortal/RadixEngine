#include <radix/env/Environment.hpp>

#include <stdexcept>
#include <getopt.h>
#include <iostream>
#include <vector>

#include <radix/core/file/Path.hpp>
#include <radix/env/Util.hpp>

namespace radix {

Config Environment::config;

std::string Environment::datadir = "";

/** @class Environment
    @brief Manager for environment and config

    Manages environment variables and configuration data. Get an instance of ConfigFileParser.
*/

void Environment::init() {
  // default installation dir
  if (datadir.empty()) {
#ifndef _WIN32
    std::vector<std::string> datadirpaths = {
      "data",
      "/usr/local/share/glportal/data",
      "/usr/share/glportal/data"
    };
#else
    std::vector<std::string> datadirpaths = {
      "data"
    };
#endif

    for (std::vector<std::string>::iterator it = datadirpaths.begin(); it != datadirpaths.end(); ++it) {
      if (Path::DirectoryExist(*it)) {
        datadir = *it;
        Util::Log(Info, "DataDir") << *it;

        break;
      }
    }

  }
  initializeConfig();
}

Config& Environment::getConfig() {
  return config;
}

void Environment::initializeConfig() {
  config.load();
}

std::string Environment::getDataDir() {
  return datadir;
}

void Environment::setDataDir(const std::string &string) {
  datadir = string;
}

} /* namespace radix */
