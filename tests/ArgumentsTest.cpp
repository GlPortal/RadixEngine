#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <catch.hpp>
#include <radix/environment/Arguments.hpp>

using namespace radix;
using namespace std;

struct ArgumentsFixtures
{
  const int argumentCount;
  //  char** argumentVector[2];
  //  const char** argumentVector;
  //  char argumentVectorDummy[2][100] = {{0}};

  ArgumentsFixtures(): argumentCount(3) {
    //    strncpy (argumentVectorDummy[0],"executableName", strlen("executableName")+1);
    //    argumentVector = argumentVectorDummy;
    //    argumentVector
    //argumentVector = { "param0", "param1", "param2" };
  }

  ~ArgumentsFixtures() {}

};

TEST_CASE_METHOD(ArgumentsFixtures, "Parameter extraction working", "[arguments]") {
  /*  SECTION("Get Data Path") {
    radix::Arguments arguments = Arguments(argumentCount, argumentVector);
    REQUIRE(arguments.getDataPath() == "/tmp");
    }*/

  SECTION("Determine executable name") {
    const char* argumentVector[] = { "executableName", "param1", "param2" };
    // For backwards compatibility C++ and C expect the argv to be non-const.
    radix::Arguments arguments = Arguments(argumentCount, const_cast<char**>(argumentVector));
    REQUIRE(arguments.getExecutableName() == "executableName");
  }
}
