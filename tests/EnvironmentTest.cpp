#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <catch.hpp>
#include <radix/environment/Environment.hpp>
#include <stdexcept>
#include <string>

using namespace radix;
using namespace std;

/*
 * The paths used here are only compatible with unix systems
 */
struct EnvironmentFixtures
{
  std::string dataPath;

  EnvironmentFixtures() {
    dataPath = "/tmp"; // this is where we assume that /tmp exists on every unix system
  }

  ~EnvironmentFixtures() {}

};

TEST_CASE_METHOD(EnvironmentFixtures, "Environment construction sets data path correctly", "[environment]") {
  SECTION("Reading and writing of data path working") {
    radix::Environment environment = Environment(dataPath);
    REQUIRE(environment.getDataPath() == dataPath);
  }

  SECTION("Exception is thrown when data path does not exist") {
    REQUIRE_THROWS_AS(radix::Environment("/non-existant-path"), runtime_error);
  }
}
