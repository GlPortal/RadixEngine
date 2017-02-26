#define CATCH_CONFIG_MAIN
#include <catch.hpp>
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

  XmlHelperFixtures() {
    doc.NewDeclaration("1.0");
    lightElement = doc.NewElement("light");
    sourceVector = Vector3f(155, 266, 377);
    lightElement->SetAttribute("x", sourceVector.x);
    lightElement->SetAttribute("y", sourceVector.y);
    lightElement->SetAttribute("z", sourceVector.z);
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
