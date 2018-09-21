#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <catch2/catch.hpp>
#include <tinyxml2.h>
#include <radix/core/math/Vector3f.hpp>
#include <radix/data/map/XmlHelper.hpp>
#include <stdexcept>

using namespace radix;
using namespace std;

struct XmlHelperFixtures
{
  Vector3f sourceVector;
  Vector3f vector;
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement * lightElement;
  tinyxml2::XMLElement * testElement;

  XmlHelperFixtures() {
    doc.NewDeclaration("1.0");
    lightElement = doc.NewElement("light");
    sourceVector = Vector3f(155, 266, 377);
    lightElement->SetAttribute("x", sourceVector.x);
    lightElement->SetAttribute("y", sourceVector.y);
    lightElement->SetAttribute("z", sourceVector.z);

    testElement = doc.NewElement("test");
    testElement->SetAttribute("key", "value");
    testElement->SetAttribute("emptyKey", "");

  }

  ~XmlHelperFixtures() {}

};

TEST_CASE_METHOD(XmlHelperFixtures, "Data can be extracted from xml", "[xml-helper]") {
  SECTION("Extracted vectors are valid") {
    XmlHelper::pushAttributeVertexToVector(lightElement, vector);

    bool vectorIsValid(false);
    Vector3f resultVector = sourceVector - vector;
    if((resultVector.x + resultVector.y + resultVector.z) == 0 ){
      vectorIsValid = true;
    }
    REQUIRE(vectorIsValid);
  }

  SECTION("Extracting missing attribute throws error") {
    lightElement->DeleteAttribute("z");
    REQUIRE_THROWS_AS(XmlHelper::pushAttributeVertexToVector(lightElement, vector), runtime_error);
  }
}

TEST_CASE_METHOD(XmlHelperFixtures, "String attributes can be extracted from xml", "[xml-string-attribute]") {
  SECTION("Non mandatory string can be extracted") {

    REQUIRE(XmlHelper::extractStringAttribute(testElement, "key") == "value");
  }

  SECTION("Mandatory string can be extracted") {

    REQUIRE(XmlHelper::extractStringMandatoryAttribute(testElement, "key") == "value");
  }

  SECTION("Error is thrown when Mandatory string does not exist") {

    REQUIRE_THROWS_AS(XmlHelper::extractStringMandatoryAttribute(testElement, "non-existant"), runtime_error);
  }
}
