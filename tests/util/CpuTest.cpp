#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "radix/util/DebugInfo.hpp"
#include "testutility.hpp"

#ifdef _MSC_VER
constexpr auto CpuModelCommand    = "echo GenuineIntel";
constexpr auto CpuVendorIdCommand = "echo 1";
constexpr auto CpuCacheCommand    = "echo 32kB";
constexpr auto CpuCoresCommand    = "echo 1";
constexpr auto CpuHyperthreadingCountCommand = "echo 1";
#else
constexpr auto CpuModelCommand = "awk -F: '/model name/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' "
"/proc/cpuinfo";
constexpr auto CpuVendorIdCommand = "awk -F: '/vendor_id/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' "
"/proc/cpuinfo";
constexpr auto CpuCacheCommand = "awk -F: '/cache/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' "
"/proc/cpuinfo";
constexpr auto CpuCoresCommand = "awk -F: '/cpu cores/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' "
"/proc/cpuinfo";
constexpr auto CpuHyperthreadingCountCommand = "grep -c \"processor\" /proc/cpuinfo";
#endif

TEST_CASE("CPU Model", "[CpuModel]") {
  auto CPU_Name = getCommand(CpuModelCommand);
  REQUIRE(DebugInfo::getCPU_Name() == CPU_Name);
}

TEST_CASE("CPU Vendor", "[CpuVendorId]") {
  auto CPU_Vendor = getCommand(CpuVendorIdCommand);
  REQUIRE(DebugInfo::getCPU_Vendor() == CPU_Vendor);
}

TEST_CASE("CPU Cache", "[CpuCache]") {
  auto CPU_Cache = getCommand(CpuCacheCommand);
  REQUIRE(DebugInfo::getCPU_Cache() == CPU_Cache);
}

TEST_CASE("CPU Cores", "[CpuCores]") {
  auto CPU_Cores = getCommand(CpuCoresCommand);
  REQUIRE(DebugInfo::getCPU_Cores() == CPU_Cores);
}

TEST_CASE("CPU Hyperthreading count", "[CpuHyperthreadingCount]") {
  auto hThreadCount = getCommand(CpuHyperthreadingCountCommand);
  REQUIRE(DebugInfo::getCPU_hyperThreadingCount() == hThreadCount);
}
