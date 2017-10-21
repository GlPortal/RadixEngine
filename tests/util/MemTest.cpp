#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "radix/util/DebugInfo.hpp"
#include "testutility.hpp"


#if defined _MSC_VER
constexpr auto MemTotalCommand  = "echo 4095MB";
constexpr auto SwapTotalCommand = "echo 5503MB";
constexpr auto MemFreeCommand   = "echo 1";
#else
constexpr auto MemTotalCommand = "grep MemTotal /proc/meminfo | awk -F: '{x=$2} END {sub(/^[ ]+/, \"\", "
"x); print x}'";
constexpr auto SwapTotalCommand = "grep SwapTotal /proc/meminfo | awk -F: '{x=$2} END {sub(/^[ ]+/, \"\", "
"x); print x}'";
constexpr auto MemFreeCommand = "grep MemFree /proc/meminfo | awk -F: '{x=$2}"
"END {sub(/^[ ]+/, \"\", x); print x}'";
#endif


TEST_CASE("Memory Total", "[MemTotal]") {
	auto systemCommand = getCommand(MemTotalCommand);
  REQUIRE(DebugInfo::getMem_Total() == systemCommand);
}


TEST_CASE("Swap Total", "[SwapTotal]") {
	auto systemCommand = getCommand(SwapTotalCommand);
  DebugInfo::isInitlized();
  auto swap = DebugInfo::mInfo[DebugInfo::Mem]["SwapTotal"];
  REQUIRE(swap == systemCommand);
}

/*
TEST_CASE("Mem Free", "[MemFree]") {
  auto systemCommand = getCommand(MemFreeCommand);
  REQUIRE(SystemInfo::getMem_Free(true) == systemCommand);
}
*/
