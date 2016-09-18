#include <radix/env/Environment.hpp>

#include <stdexcept>
#include <getopt.h>
#include <iostream>
#include <vector>

#include <radix/core/file/Path.hpp>
#include <radix/env/Util.hpp>
#include <radix/env/OperatingSystem.hpp>

namespace radix {

Config Environment::config;

std::string Environment::dataDir = "";

void Environment::init() {
  if (dataDir.empty()) {
    std::vector<std::string> dataDirPaths = {
      "data"
    };

    if(OperatingSystem::IsLinux()){
      dataDirPaths.push_back("/usr/local/share/glportal/data");
      dataDirPaths.push_back("/usr/share/glportal/data");
    }

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
