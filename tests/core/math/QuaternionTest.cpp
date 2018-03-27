#include <random>
#include <cmath>
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <catch.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Quaternion.hpp>

#include <iostream>

using namespace radix;
using namespace std;

struct QuaternionTestFixtures {
  Quaternion quat;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<> nppi2, nppi, z360;

  QuaternionTestFixtures() :
    gen(rd()),
    nppi2(-M_PI/2 + 0.002f, M_PI/2 - 0.002f),
    nppi(-M_PI + 0.002f, M_PI - 0.002f),
    z360(0, 359.99f) {
  }

  ~QuaternionTestFixtures() {}

};

bool fuzzyEq(float f1, float f2) {
  return std::abs(f1-f2) <= 0.0001f;
}

bool fuzzyEq(const Quaternion &q, float x, float y, float z, float w) {
  return fuzzyEq(q.x, x) and fuzzyEq(q.y, y) and fuzzyEq(q.z, z) and fuzzyEq(q.w, w);
}

bool fuzzyEq(const Quaternion &q, const Quaternion &k) {
  return fuzzyEq(q.x, k.x) and fuzzyEq(q.y, k.y) and fuzzyEq(q.z, k.z) and fuzzyEq(q.w, k.w);
}

void printq(const Quaternion &q) {
  printf("%f %f %f %f\n", q.x, q.y, q.z, q.w);
}

TEST_CASE_METHOD(QuaternionTestFixtures, "Angle to Quaternion conversion", "[quaternion]") {
  // euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
  SECTION("Convert ax x 90 to quat") {
    quat.fromAxAngle(1, 0, 0, rad(90));
    CHECK(fuzzyEq(quat, 0.7071, 0, 0, 0.7071));
  }
  SECTION("Convert ax x 45 to quat") {
    quat.fromAxAngle(1, 0, 0, rad(45));
    CHECK(fuzzyEq(quat, 0.3827, 0, 0, 0.9239));
  }
  SECTION("Convert ax y 90 to quat") {
    quat.fromAxAngle(0, 1, 0, rad(90));
    CHECK(fuzzyEq(quat, 0, 0.7071, 0, 0.7071));
  }
  SECTION("Convert ax y 45 to quat") {
    quat.fromAxAngle(0, 1, 0, rad(45));
    CHECK(fuzzyEq(quat, 0, 0.3827, 0, 0.9239));
  }
  SECTION("Convert ax xy 90 to quat") {
    quat.fromAxAngle(1, 1, 0, rad(90));
    CHECK(fuzzyEq(quat, 0.5, 0.5, 0, 0.7071));
  }
  SECTION("Convert ax xy 45 to quat") {
    quat.fromAxAngle(1, 1, 0, rad(45));
    CHECK(fuzzyEq(quat, 0.2706, 0.2706, 0, 0.9239));
  }

  // euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/steps/
  SECTION("Convert to quat heading 90") {
    quat.fromAero(rad(90), 0, 0);
    CHECK(fuzzyEq(length(quat), 1));
    CHECK(fuzzyEq(quat, 0, 0.7071, 0, 0.7071));
  }
  SECTION("Convert to quat attitude 90") {
    quat.fromAero(0, rad(90), 0);
    CHECK(fuzzyEq(length(quat), 1));
    CHECK(fuzzyEq(quat, 0.7071, 0, 0, 0.7071));
  }
  SECTION("Convert to quat heading attitude 90") {
    quat.fromAero(rad(90), rad(90), 0);
    CHECK(fuzzyEq(length(quat), 1));
    CHECK(fuzzyEq(quat, 0.5, 0.5, -0.5, 0.5));
  }
  SECTION("Convert quat to aero north singularity ") {
    Vector3f init, back;
    for (int n = 0; n < 50; ++n) {
      init.x = nppi(gen);
      init.y = rad(90);
      init.z = 0;
      back = quat.fromAero(init).toAero();
      std::cout << init.x << ' ' << init.y << ' ' << init.z << " -> " << back.x << ' ' << back.y << ' ' << back.z << std::endl;
      CHECK(back.fuzzyEqual(init));
    }
  }
  SECTION("Convert quat to aero south singularity ") {
    Vector3f init, back;
    for (int n = 0; n < 50; ++n) {
      init.x = nppi(gen);
      init.y = rad(-90);
      init.z = 0;
      back = quat.fromAero(init).toAero();
      std::cout << init.x << ' ' << init.y << ' ' << init.z << " -> " << back.x << ' ' << back.y << ' ' << back.z << std::endl;
      CHECK(back.fuzzyEqual(init));
    }
  }
  SECTION("Convert quat to aero a 90 ") {
    quat = Quaternion(.7071, 0, 0, .7071);
    Vector3f a = quat.toAero();
    CHECK(a.fuzzyEqual(Vector3f(0, rad(90), 0)));
  }
  SECTION("Convert quat to aero ha ") {
    Vector3f init, back;
    for (int n = 0; n < 100; ++n) {
      init.x = nppi(gen);
      init.y = nppi2(gen);
      init.z = 0;
      back = quat.fromAero(init).toAero();
      std::cout << init.x << ' ' << init.y << ' ' << init.z << " -> " << back.x << ' ' << back.y << ' ' << back.z << std::endl;
      CHECK(back.fuzzyEqual(init));
    }
  }
  SECTION("Convert random aero to quat to aero ht") {
    Vector3f init, back;
    for (int n = 0; n < 100; ++n) {
      init.x = nppi(gen);
      init.y = 0;
      init.z = nppi(gen);
      back = quat.fromAero(init).toAero();
      std::cout << init.x << ' ' << init.y << ' ' << init.z << " -> " << back.x << ' ' << back.y << ' ' << back.z << std::endl;
      CHECK(back.fuzzyEqual(init));
    }
  }
  SECTION("Convert random head rot h 90") {
    Vector3f init, back;
    Quaternion q; q.fromAxAngle(0, 1, 0, rad(90));
    for (int n = 0; n < 100; ++n) {
      init.x = nppi(gen);
      init.y = nppi2(gen);
      init.z = 0;
      back = (q*quat.fromAero(init)).toAero();
      init.x = std::remainder(init.x + M_PI + M_PI/2, 2*M_PI)-M_PI;
      back.x = std::remainder(back.x + M_PI, 2*M_PI)-M_PI;
      CHECK(back.fuzzyEqual(init));
    }
  }
  SECTION("Convert random head rot h rand") {
    Vector3f init, back; float add;
    Quaternion q;
    for (int n = 0; n < 200; ++n) {
      add = rad(z360(gen));
      q.fromAxAngle(0, 1, 0, add);
      init.x = nppi(gen);
      init.y = nppi2(gen);
      init.z = 0;
      back = (q*quat.fromAero(init)).toAero();
      init.x = std::remainder(init.x + M_PI + add, 2*M_PI)-M_PI;
      back.x = std::remainder(back.x + M_PI, 2*M_PI)-M_PI;
      CHECK(back.fuzzyEqual(init));
    }
  }
  SECTION("Convert random head rot h rand add sub") {
    Vector3f init, back; float add, sub;
    Quaternion q, qs;
    for (int n = 0; n < 200; ++n) {
      add = rad(z360(gen));
      sub = rad(z360(gen));
      q.fromAxAngle(0, 1, 0, add);
      qs.fromAxAngle(0, 1, 0, -sub);
      init.x = nppi(gen);
      init.y = nppi2(gen);
      init.z = 0;
      back = (q*qs*quat.fromAero(init)).toAero();
      init.x = std::remainder(init.x + M_PI + add - sub, 2*M_PI)-M_PI;
      back.x = std::remainder(back.x + M_PI, 2*M_PI)-M_PI;
      CHECK(back.fuzzyEqual(init));
    }
  }
  SECTION("Convert random head rot h rand add sub by inv") {
    Vector3f init, back; float add, sub;
    Quaternion q, qs;
    for (int n = 0; n < 200; ++n) {
      add = rad(z360(gen));
      sub = rad(z360(gen));
      q.fromAxAngle(0, 1, 0, add);
      qs.fromAxAngle(0, 1, 0, sub);
      init.x = nppi(gen);
      init.y = nppi2(gen);
      init.z = 0;
      back = (q*conjugate(qs)*quat.fromAero(init)).toAero();
      init.x = std::remainder(init.x + M_PI + add - sub, 2*M_PI)-M_PI;
      back.x = std::remainder(back.x + M_PI, 2*M_PI)-M_PI;
      CHECK(back.fuzzyEqual(init));
    }
  }
}
