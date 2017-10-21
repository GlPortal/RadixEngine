#include "radix/util/linux/hardwareInfo.hpp"
#include <fstream>

using map = HardwareInfo::map;

bool HardwareInfo::read_cpuInfo(map& info) {
  //if (!info.empty()) return true;
  std::ifstream cpuInfo("/proc/cpuinfo");

  if (cpuInfo.is_open()) {
    std::string line;
    int hyperThreads = 0;
    while (std::getline(cpuInfo, line)) {
      if (line.find("model name") != std::string::npos) {
        auto pos = line.find(":");
        info["modelName"] = line.substr(pos + 2, line.size());
      } else if (line.find("vendor_id") != std::string::npos) {
        auto pos = line.find(":");
        info["vendorId"] = line.substr(pos + 2, line.size());
      } else if (line.find("cache size") != std::string::npos) {
        auto pos = line.find(":");
        info["cacheSize"] = line.substr(pos + 2, line.size());
      } else if (line.find("cpu cores") != std::string::npos) {
        auto pos = line.find(":");
        info["cpuCores"] = line.substr(pos + 2, line.size());
      } else if (line.find("processor") != std::string::npos) {
        hyperThreads++;
      }
    }
    info["hyperThreadsCount"] = std::to_string(hyperThreads);
    return true;
  }
  return false;
}

bool HardwareInfo::read_memInfo(map& info) {
  std::ifstream memFile("/proc/meminfo");
  if (memFile.is_open()) {
    std::string line;
    while (std::getline(memFile, line)) {
      if (line.find("MemTotal") != std::string::npos) {
        auto pos = line.find(":") + 1;
        while (line[pos] == ' ') pos++;
        info["MemTotal"] = line.substr(pos, line.size());
      }
      if (line.find("SwapTotal") != std::string::npos) {
        auto pos = line.find(":") + 1;
        while (line[pos] == ' ') pos++;
        info["SwapTotal"] = line.substr(pos, line.size());
      }
      if (line.find("MemFree") != std::string::npos) {
        auto pos = line.find(":") + 1;
        while (line[pos] == ' ') pos++;
        info["MemFree"] = line.substr(pos, line.size());
      }
    }
    return true;
  }
  return false;
}
