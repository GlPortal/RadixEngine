#include "radix/util/linux/distroInfo.hpp"
#include <fstream>

using map = DistroInfo::map;

bool DistroInfo::read_distroInfo(std::string&& fileName, map& info) {
  if (!info.empty()) return true;
  std::ifstream iFile(fileName);
  if (iFile.is_open()) {
    std::string line;
    while (std::getline(iFile, line)) {
      auto pos = line.find("=");
      if (pos != std::string::npos) {
        info[line.substr(0, pos)] = line.substr(pos + 1, line.size());
      }
    }
    return true;
  }
  return false;
}
