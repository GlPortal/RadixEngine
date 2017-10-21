#ifndef HARDWARE_INFO_HPP
#define HARDWARE_INFO_HPP
#include <unordered_map>
#include <string>

struct HardwareInfo {
  using map = std::unordered_map<std::string, std::string>;

  static bool read_cpuInfo(map& info);

  static bool read_memInfo(map& info);
};

#endif  //! HARDWARE_INFO_HPP
