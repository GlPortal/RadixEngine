#ifndef DISTRO_INFO_HPP
#define DISTRO_INFO_HPP
#include <unordered_map>
#include <string>

struct DistroInfo {
  using map = std::unordered_map<std::string, std::string>;
  static bool read_distroInfo(std::string&& fileName, map& info);
};
#endif  //! DISTRO_INFO_HPP
