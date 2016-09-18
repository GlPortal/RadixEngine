#include <radix/env/Environment.hpp>

#include <stdexcept>
#include <getopt.h>
#include <iostream>
#include <vector>

#include <radix/core/file/Path.hpp>
#include <radix/env/Util.hpp>

namespace radix {

Config Environment::config;

std::string Environment::dataDir = "";

void Environment::init() {
  // default installation dir
  if (dataDir.empty()) {
#ifndef _WIN32
    std::vector<std::string> dataDirPaths = {
      "data",
      "/usr/local/share/glportal/data",
      "/usr/share/glportal/data"
    };
#else
    std::vector<std::string> dataDirPaths = {
      "data"
    };
#endif

    for (std::vector<std::string>::iterator it = dataDirPaths.begin(); it != dataDirPaths.end(); ++it) {
      if (Path::DirectoryExist(*it)) {
        dataDir = *it;
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
  return dataDir;
}

void Environment::setDataDir(const std::string &string) {
  dataDir = string;
}

} /* namespace radix */
