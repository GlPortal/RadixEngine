#include <radix/core/math/Math.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Quaternion.hpp>
#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include<iostream>

using namespace radix;
using namespace std;

struct MathTestFixtures {
  Quaternion quat;
  float PI = 3.14159265358979323846f;
};

bool fuzzyEq(float f1, float f2) {
  return std::abs(f1-f2) <= 0.001f;
}

bool fuzzyEq(const Quaternion &q, float x, float y, float z, float w) {
  return fuzzyEq(q.x, x) && fuzzyEq(q.y, y) && fuzzyEq(q.z, z) && fuzzyEq(q.w, w);
}

bool fuzzyEq(const Quaternion &q, const Quaternion &k) {
  return fuzzyEq(q.x, k.x) && fuzzyEq(q.y, k.y) && fuzzyEq(q.z, k.z) && fuzzyEq(q.w, k.w);
}

void printq(const Quaternion &q) {
  printf("%f %f %f %f\n", q.x, q.y, q.z, q.w);
}

TEST_CASE("Clamp test") {
  // value between bounds
  REQUIRE(Math::clamp(3, 1, 6) == 3);
  // value below lower bound
  REQUIRE(Math::clamp(1, 3, 5) == 3);
  // value above upper bound
  REQUIRE(Math::clamp(6, 2, 4) == 4);
  // negative stuff, zeros
}


TEST_CASE("Sign test", "[sign]") {
  // value positive
  REQUIRE(Math::sign(3) == 1);
  // value negative
  REQUIRE(Math::sign(-3) == -1);
  // value zero
  REQUIRE(Math::sign(0) == 0);
}

// converts deg to rad
TEST_CASE_METHOD(MathTestFixtures, "Rad test", "[rad]") {
  REQUIRE(fuzzyEq(rad(0), 0));
  REQUIRE(fuzzyEq(rad(90), PI/2));
  REQUIRE(fuzzyEq(rad(180), PI));
  REQUIRE(fuzzyEq(rad(270), 3*PI/2));
  REQUIRE(fuzzyEq(rad(360), 2*PI));
}

// converts rad to deg
TEST_CASE_METHOD(MathTestFixtures, "Deg test", "[deg]") {
  REQUIRE(fuzzyEq(deg(0), 0));
  REQUIRE(fuzzyEq(deg(PI/2), 90));
  REQUIRE(fuzzyEq(deg(PI), 180));
  REQUIRE(fuzzyEq(deg(3*PI/2), 270));
  REQUIRE(fuzzyEq(deg(2*PI), 360));
}

// toDirection test
// This test should test that the toDirection function correctly rotates vectors according
// to a quaternion input. We will test that this is true for a rotation to each axis and to
// a position within each quadrant
TEST_CASE_METHOD(MathTestFixtures, "toDirection test") {
  SECTION("to backward") {
    // first create a quat from axis angle representation, use toDirection to make it
    // a vector and that vector should be (0, 0, 1)
    quat.fromAxAngle(1, 0, 0, rad(180));
    Vector3f direction = Math::toDirection(quat);
  // now test whether the rotation imparted by the quaternion
  // is correct for a forward facing vector which is (0, 0, -1)
  // we are rotating to backwards, which is (0, 0, 1)
    Vector3f backward(0, 0, 1);
    REQUIRE(direction.fuzzyEqual(backward));
  }

  SECTION("to forward") {
    quat.fromAxAngle(1, 0, 0, rad(0));
    Vector3f direction = Math::toDirection(quat);
    Vector3f forward(0, 0, -1);
    REQUIRE(direction.fuzzyEqual(forward));
  }

  SECTION("to right") {
    quat.fromAxAngle(0, 1, 0, rad(-90));
    Vector3f direction = Math::toDirection(quat);
    Vector3f right(1, 0, 0);
    REQUIRE(direction.fuzzyEqual(right));
  }
  SECTION("to left") {
    quat.fromAxAngle(0, 1, 0, rad(90));
    Vector3f direction = Math::toDirection(quat);
    Vector3f left(-1, 0, 0);
    REQUIRE(direction.fuzzyEqual(left));
  }
  SECTION("to up") {
    quat.fromAxAngle(1, 0, 0, rad(90));
    Vector3f direction = Math::toDirection(quat);
    Vector3f up(0, 1, 0);
    REQUIRE(direction.fuzzyEqual(up));
  }
  SECTION("to down") {
    quat.fromAxAngle(1, 0, 0, rad(-90));
    Vector3f direction = Math::toDirection(quat);
    Vector3f down(0, -1, 0);
    REQUIRE(direction.fuzzyEqual(down));
  }



  // Now each quadrant (there are eight)
  SECTION("to first quadrant") {
    quat.fromAxAngle(1, -1, 0, rad(135));
    Vector3f direction = Math::toDirection(quat);
    Vector3f first(.5, .5, .7071);
    REQUIRE(direction.fuzzyEqual(first));
  }
  SECTION("to second quadrant") {
    quat.fromAxAngle(1, 1, 0, rad(135));
    Vector3f direction = Math::toDirection(quat);
    Vector3f second(-.5, .5, .7071);
    REQUIRE(direction.fuzzyEqual(second));
  }
  SECTION("to third quadrant") {
    quat.fromAxAngle(-1, 1, 0, rad(135));
    Vector3f direction = Math::toDirection(quat);
    Vector3f third(-.5, -.5, .7071);
    REQUIRE(direction.fuzzyEqual(third));
  }
  SECTION("to fourth quadrant") {
    quat.fromAxAngle(-1, -1, 0, rad(135));
    Vector3f direction = Math::toDirection(quat);
    Vector3f fourth(.5, -.5, .7071);
    REQUIRE(direction.fuzzyEqual(fourth));
  }
  SECTION("to fifth quadrant") {
    quat.fromAxAngle(1, -1, 0, rad(45));
    Vector3f direction = Math::toDirection(quat);
    Vector3f fifth(.5, .5, -.7071);
    REQUIRE(direction.fuzzyEqual(fifth));
  }
  SECTION("to sixth quadrant") {
    quat.fromAxAngle(1, 1, 0, rad(45));
    Vector3f direction = Math::toDirection(quat);
    Vector3f sixth(-.5, .5, -.7071);
    REQUIRE(direction.fuzzyEqual(sixth));
  }
  SECTION("to seventh quadrant") {
    quat.fromAxAngle(-1, 1, 0, rad(45));
    Vector3f direction = Math::toDirection(quat);
    Vector3f seventh(-.5, -.5, -.7071);
    REQUIRE(direction.fuzzyEqual(seventh));
  }
  SECTION("to eighth quadrant") {
    quat.fromAxAngle(-1, -1, 0, rad(45));
    Vector3f direction = Math::toDirection(quat);
    Vector3f eighth(.5, -.5, -.7071);
    REQUIRE(direction.fuzzyEqual(eighth));
  }
}

TEST_CASE_METHOD(MathTestFixtures, "ToEuler test") {
  SECTION("no rotation") {
    Vector3f direction(0, 0, -1);
    Vector3f euler = Math::toEuler(direction);
    Vector3f correct(0, 0, 0);
    REQUIRE(euler.fuzzyEqual(correct, .001));
  }
  SECTION("antiparallel") {
    Vector3f direction(0, 0, 1);
    Vector3f euler = Math::toEuler(direction);
    Vector3f correct(0, rad(180), 0);
    REQUIRE(euler.fuzzyEqual(correct, .001));
  }
  SECTION("up") {
    Vector3f direction(0, 1, 0);
    Vector3f euler = Math::toEuler(direction);
    Vector3f correct(rad(90), 0, 0);
    REQUIRE(euler.fuzzyEqual(correct, .001));
  }
  SECTION("down") {
    Vector3f direction(0, -1, 0);
    Vector3f euler = Math::toEuler(direction);
    Vector3f correct(rad(-90), 0, 0);
    REQUIRE(euler.fuzzyEqual(correct, .001));
  }
  SECTION("right") {
    Vector3f direction(1, 0, 0);
    Vector3f euler = Math::toEuler(direction);
    Vector3f correct(0, rad(270), 0);
    REQUIRE(euler.fuzzyEqual(correct, .001));
  }
  SECTION("left") {
    Vector3f direction(-1, 0, 0);
    Vector3f euler = Math::toEuler(direction);
    Vector3f correct(0, rad(90), 0);
    REQUIRE(euler.fuzzyEqual(correct, .001));
  }
}
