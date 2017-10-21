#include "radix/util/linux/kernelInfo.hpp"
#include <sys/utsname.h>

using map = KernelInfo::map;

bool KernelInfo::read_uname(map& info) {
  if (!info.empty()) return true;
  utsname kernelInfo;

  if (uname(&kernelInfo) == -1) return false;

  info["sysname"] = kernelInfo.sysname;
  info["nodename"] = kernelInfo.nodename;
  info["release"] = kernelInfo.release;
  info["version"] = kernelInfo.version;
  info["machine"] = kernelInfo.machine;
  return true;
}
