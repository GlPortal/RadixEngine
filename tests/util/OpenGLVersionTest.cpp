#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "radix/util/DebugInfo.hpp"
#include "testutility.hpp"

#ifdef _WIN32
constexpr auto command = "echo \"1.1.0\"";
#else
constexpr auto command = "glxinfo -B | awk -F: '/OpenGL version string/{ sub(/^[ ]+/, \"\", $2); "
"print $2 }'";
#endif // _WIN32


TEST_CASE("OpenGL Version", "[OpenGLVersion]") {
  auto OpenGL_Version = getCommand(command);
#ifdef _WIN32
  OpenGL_Version = OpenGL_Version.substr(1, OpenGL_Version.size() - 2);
#endif
  REQUIRE(DebugInfo::getOpenGL_Version() == OpenGL_Version);
}
