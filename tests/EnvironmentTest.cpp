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
    dataPath = "/mock/path";
  }

  ~EnvironmentFixtures() {}

};

TEST_CASE_METHOD(EnvironmentFixtures, "Environment construction", "[environment]") {
  SECTION("Reading and writing of data path working") {
    radix::Environment environment = Environment(dataPath);
    bool passed = false;
    if(environment.getDataPath() == dataPath) {
      passed = true;
    }
    REQUIRE(passed);
  }
}
