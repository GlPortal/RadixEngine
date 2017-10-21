#ifndef KERNEL_INFO_HPP
#define KERNEL_INFO_HPP
#include <string>
#include <unordered_map>

struct KernelInfo {
  using map = std::unordered_map<std::string, std::string>;

  static bool read_uname(map& info);
};

#endif  //! KERNEL_INFO_HPP
