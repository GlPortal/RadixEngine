#include "radix/util/DebugInfo.hpp"

#ifdef _WINDOWS
#if defined(_MSC_VER)
#include "windows\systemInfo.hpp"
#elif defined(__MINGW32__)
#endif //!_MSC_VER
#include "radix/util/windows/systemInfo.hpp
#else
#include "radix/util/linux/kernelInfo.hpp"
#include "radix/util/linux/hardwareInfo.hpp"
#include "radix/util/linux/distroInfo.hpp"
#include "radix/util/linux/glibcInfo.hpp"
#endif //!_WINDOWS
#include "radix/util/sdlInfo.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

std::array<DebugInfo::stringMap, DebugInfo::COUNT> DebugInfo::mInfo;

using map = std::unordered_map<std::string, std::string>;


bool DebugInfo::isInitlized() {
  auto& kernelInfo = mInfo[DebugInfo::Kernel];
  auto& cpuInfo    = mInfo[DebugInfo::CPU];
  auto& memInfo    = mInfo[DebugInfo::Mem];
  auto& distroInfo = mInfo[DebugInfo::Distro];
  auto& sdlInfo    = mInfo[DebugInfo::SDL_Version];
  auto& disInfo    = mInfo[DebugInfo::DisplayServer];
  auto& GPUInfo    = mInfo[DebugInfo::GPU];
  auto& glInfo     = mInfo[DebugInfo::OpenGL];
  auto& glibcInfo  = mInfo[DebugInfo::GLibC];
  SdlInfo::read_SDL(sdlInfo, disInfo, glInfo, GPUInfo, cpuInfo, memInfo);
  SdlInfo::destory_SDL();

#ifdef _MSC_VER
  glibcInfo["version"] = std::to_string(_MSC_VER);
#else
  // Read libc Info
  GlibcInfo::read_glibc(glibcInfo);
#endif

#ifdef _WINDOWS
  systemInfo::readSystemInfo(kernelInfo, cpuInfo, memInfo);

  distroInfo["NAME"]       = "Windows";
  distroInfo["VERSION_ID"] = systemInfo::getWindowsName();
#else
  // Read Kernel Info
  KernelInfo::read_uname(kernelInfo);
  // Read CPU Info
  HardwareInfo::read_cpuInfo(cpuInfo);
  // Read Mem Info
  HardwareInfo::read_memInfo(memInfo);
  // Read Distro Info
  if (!DistroInfo::read_distroInfo("/etc/os-release", distroInfo)) {
    DistroInfo::read_distroInfo("/etc/lsb-release", distroInfo);
  }
#endif
  return true;
}


std::string DebugInfo::getKernel_Name() {
  auto& kernel = mInfo[DebugInfo::Kernel];
  isInitlized();

  return kernel["sysname"];
}

std::string DebugInfo::getKernel_Arch() {
  auto& kernel = mInfo[DebugInfo::Kernel];
  isInitlized();

  return kernel["machine"];
}

std::string DebugInfo::getKernel_Release() {
  auto& kernel = mInfo[DebugInfo::Kernel];
  isInitlized();

  return kernel["release"];
}

std::string DebugInfo::getMem_Total() {
  auto& mem = mInfo[DebugInfo::Mem];
  isInitlized();

  return mem["MemTotal"];
}

std::string DebugInfo::getMem_Free(bool forceToReload) {
  auto& mem = mInfo[DebugInfo::Mem];
#ifdef _MSC_VER
#else
  if (forceToReload) {
    HardwareInfo::read_memInfo(mem);
  } else {
    isInitlized();
  }
#endif
  return mem["MemFree"];
}

std::string DebugInfo::getCPU_Name() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["modelName"];
}

std::string DebugInfo::getCPU_Vendor() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["vendorId"];
}

std::string DebugInfo::getCPU_Cache() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["cacheSize"];
}

std::string DebugInfo::getCPU_Cores() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["cpuCores"];
}

std::string DebugInfo::getCPU_hyperThreadingCount() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["hyperThreadsCount"];
}

std::string DebugInfo::getOS_Name() {
  if (!isInitlized()) return "Unknown";

  auto& kernel = mInfo[DebugInfo::Distro];

  map::iterator itr;
  if ((itr = kernel.find("NAME")) != kernel.end()) {
    return itr->second;
  } else if ((itr = kernel.find("DISTRIB_ID")) != kernel.end()) {
    return itr->second;
  }

  return "Unknown";
}

std::string DebugInfo::getOS_Version() {
  if (!isInitlized()) return "Unknown";

  auto& version = mInfo[DebugInfo::Distro];

  map::iterator itr;
  if ((itr = version.find("VERSION_ID")) != version.end()) {
    return itr->second;
  } else if ((itr = version.find("DISTRIB_RELEASE")) != version.end()) {
    return itr->second;
  }
  return "Unknown";
}

std::string DebugInfo::getLibC_Version() {
  auto& glib = mInfo[DebugInfo::GLibC];
  if (!isInitlized()) return "Unknow";
  return glib["version"];
}

std::string DebugInfo::getDisplayServer() {
  auto& displayServer = mInfo[DebugInfo::DisplayServer];
  if (!isInitlized()) return "Unknow";
  return displayServer["subsystem"];
}

std::string DebugInfo::getSDL_Version() {
  auto& SDL = mInfo[DebugInfo::SDL_Version];
  if (!isInitlized()) return "Unknow";
  return SDL["version"];
}

std::string DebugInfo::getGPU_Name() {
  auto& opengl = mInfo[DebugInfo::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["glRenderer"];
}

std::string DebugInfo::getGPU_Vendor() {
  auto& opengl = mInfo[DebugInfo::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["glVendor"];
}

std::string DebugInfo::getGPU_Size() {
  auto& opengl = mInfo[DebugInfo::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["size"];
}

std::string DebugInfo::getOpenGL_Version() {
  auto& opengl = mInfo[DebugInfo::OpenGL];
  if (!isInitlized()) return "Unknow";
  return opengl["glVersion"];
}
