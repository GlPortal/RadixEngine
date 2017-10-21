#include "radix/util/linux/glibcInfo.hpp"
#include <gnu/libc-version.h>

using map = GlibcInfo::map;

bool GlibcInfo::read_glibc(map& info) {
  info["version"] = gnu_get_libc_version();
  return !info["version"].empty();
}
