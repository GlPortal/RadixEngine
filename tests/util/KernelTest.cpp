#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "radix/util/DebugInfo.hpp"
#include "testutility.hpp"

#ifdef _WIN32
constexpr auto KernelNameCommand    = "echo";
constexpr auto CPU_ArchCommand      = "echo";
constexpr auto KernelReleaseCommand = "echo";
#else
constexpr auto KernelNameCommand = "uname";
constexpr auto CPU_ArchCommand = "uname -p";
constexpr auto KernelReleaseCommand = "uname -r";
#endif

TEST_CASE("Kernel Name", "[KernelName]") {
  auto kernelName = getCommand("uname");
  REQUIRE(DebugInfo::getKernel_Name() == kernelName);
}

TEST_CASE("CPU Arch", "[CPU_Arch]") {
  auto arch = getCommand("uname -p");
  REQUIRE(DebugInfo::getKernel_Arch() == arch);
}

TEST_CASE("Kernel Release", "[KernelRelease]") {
  auto release = getCommand("uname -r");
  REQUIRE(DebugInfo::getKernel_Release() == release);
}
