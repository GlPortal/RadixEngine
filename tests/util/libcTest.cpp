#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "radix/util/DebugInfo.hpp"
#include "testutility.hpp"

#if defined _MSC_VER
constexpr auto command = "echo 1910";
#else
constexpr auto command = "ldd --version | awk '/ldd/ {print $NF}'";
#endif

TEST_CASE("LibC Version", "[LibcVersion]") {
  auto system = getCommand(command);
  REQUIRE(DebugInfo::getLibC_Version() == system);
}
