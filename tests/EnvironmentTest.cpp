#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <catch.hpp>
#include <radix/environment/Environment.hpp>
#include <stdexcept>
#include <string>

using namespace radix;
using namespace std;

struct EnvironmentFixtures
{
  std::string dataPath;

  EnvironmentFixtures() {
    dataPath = "/tmp";
  }

  ~EnvironmentFixtures() {}

};

TEST_CASE_METHOD(EnvironmentFixtures, "Environment construction sets data path correctly", "[environment]") {
  SECTION("Reading and writing of data path working") {
    radix::Environment environment = Environment(dataPath);
    REQUIRE(environment.getDataPath() == dataPath);
  }
}
