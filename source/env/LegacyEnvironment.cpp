#include <radix/env/LegacyEnvironment.hpp>

#include <iostream>

#include <radix/core/file/Path.hpp>
#include <radix/env/Util.hpp>
#include <radix/env/OperatingSystem.hpp>

namespace radix {

Config LegacyEnvironment::config;

std::string LegacyEnvironment::dataDir = "";

void LegacyEnvironment::Init() {
  if (dataDir.empty()) {
    std::vector<std::string> dataDirPaths = {
      "data"
    };

    dataDirPaths.push_back("../data");

    if(OperatingSystem::IsLinux()){
      dataDirPaths.push_back("/usr/local/share/glportal/data");
      dataDirPaths.push_back("/usr/share/glportal/data");
    }

    for (auto & path : dataDirPaths) {
      Util::Log(Info, "DataDir") << "Searching data in " << path;
      if (Path::DirectoryExist(path)) {
        dataDir = path;
        Util::Log(Info, "DataDir") << "Found data in " << path;

        break;
      }
    }
  }
  initializeConfig();
}

Config& LegacyEnvironment::getConfig() {
  return config;
}

void LegacyEnvironment::initializeConfig() {
  config.load();
}

std::string LegacyEnvironment::getDataDir() {
  if (dataDir == "") {
    Util::Log(Info, "DataDir") << "No data dir set!";
    exit(0);
  }

  return dataDir;
}

void LegacyEnvironment::setDataDir(const std::string &string) {
  Util::Log(Info, "DataDir") << "Setting data dir to " << string;
  dataDir = string;
  if (dataDir[dataDir.size() - 1] == '/') {
    dataDir = dataDir.substr(0, dataDir.length() - 1);
  }
}

} /* namespace radix */
