#ifndef GLIBC_INFO_HPP
#define GLIBC_INFO_HPP
#include <unordered_map>
#include <string>

struct GlibcInfo {
  using map = std::unordered_map<std::string, std::string>;

  static bool read_glibc(map& info);
};
#endif  //! GLIBC_INFO_HPP
