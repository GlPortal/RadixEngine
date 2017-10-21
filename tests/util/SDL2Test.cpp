#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "radix/util/DebugInfo.hpp"
#include "testutility.hpp"

#ifdef _WIN32
constexpr auto displayServer = "Microsoft Windows(TM)";
#else
constexpr auto displayServer = "X Window System";
#endif

TEST_CASE("Display Server", "[Display Server]") {
  REQUIRE(DebugInfo::getDisplayServer() == displayServer);
}

TEST_CASE("SDL2 Version", "[SDL2Version]") {
  REQUIRE(DebugInfo::getSDL_Version() == "2.0");
}
