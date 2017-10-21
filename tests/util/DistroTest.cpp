#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "radix/util/DebugInfo.hpp"
#include "testutility.hpp"

#if defined _MSC_VER
constexpr auto DistroNameStr = "echo Windows";
constexpr auto DistroVersion = "echo Windows8";
#else
constexpr auto DistroNameStr = "awk -F= '/NAME/{ print $2; exit }' /etc/os-release";
constexpr auto DistroVersion = "awk -F= '/VERSION_ID/{ print $2; exit }' /etc/os-release";
#endif

TEST_CASE("Distro Name", "[DistroName]") {
	auto system = getCommand(DistroNameStr);
  REQUIRE(DebugInfo::getOS_Name() == system);
}

TEST_CASE("Distro Version", "[DistroVersion]") {
  auto system = getCommand(DistroVersion);
  REQUIRE(DebugInfo::getOS_Version() == system);
}
