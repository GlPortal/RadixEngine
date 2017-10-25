#ifndef SYSTEM_INFO_HPP
#define SYSTEM_INFO_HPP
#pragma once
#include <unordered_map>
#include <string>
#include <Windows.h>

struct systemInfo {
	using map = std::unordered_map<std::string, std::string>;

  static std::string getWindowsName();

  static DWORD CountSetBits(ULONG_PTR bitMask);

	static void readSystemInfo(map& kernelInfo, map& cpuInfo, map& memInfo);
};
#endif //! SYSTEM_INFO_HPP