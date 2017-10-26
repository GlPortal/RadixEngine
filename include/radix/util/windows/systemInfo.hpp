#ifndef SYSTEM_INFO_HPP
#define SYSTEM_INFO_HPP
#include <string>
#include <vector>
#include <unordered_map>
#include <Windows.h>

struct systemInfo {
  using map = std::unordered_map<std::string, std::string>;

  static std::vector<std::string> readRegsitery(
      const std::string& regKey, const std::vector<std::string> keys);

  static void readCPUInfo(map& cpuInfo);

  static std::string getWindowsName();

  static DWORD CountSetBits(ULONG_PTR bitMask);

  static void readSystemInfo(map& kernelInfo, map& cpuInfo, map& memInfo);
};
#endif  //! SYSTEM_INFO_HPP
