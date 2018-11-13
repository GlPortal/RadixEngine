#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <catch.hpp>
#include <radix/environment/Path.hpp>
#include <stdexcept>
#include <string>

using namespace radix;
using namespace std;

struct PathFixtures
{
  std::string dataPath;

  PathFixtures() {
    dataPath = "/tmp";
  }

  ~PathFixtures() {}

};

TEST_CASE_METHOD(PathFixtures, "Path construction sets data path correctly", "[path]") {
  SECTION("Reading and writing of data path working") {
    radix::Path path = Path(dataPath);
    REQUIRE(path.toString() == dataPath);
  }
}
