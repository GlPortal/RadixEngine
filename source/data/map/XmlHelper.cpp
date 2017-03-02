#include <radix/data/map/XmlHelper.hpp>
#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/Health.hpp>
#include <radix/core/state/GameState.hpp>
#include <radix/SoundManager.hpp>
#include <radix/env/Environment.hpp>

using namespace std;
using namespace tinyxml2;

namespace radix {

std::string XmlHelper::mandatoryAttributeMessage("Mandatory attribute has not been defined.");
std::string XmlHelper::invalidElementMessage("pushAttributeToVector received an invalid "
                                                 "XML-Element.");
/**
  * Pushes vector coordinates from an XML-element to a Vector3f.
  * Trying to pull attributes from a non-existing element is considered an exception.
  */
void XmlHelper::pushAttributeVertexToVector(XMLElement *xmlElement, Vector3f &targetVector) {
  if (xmlElement) {
    int xQueryResult = xmlElement->QueryFloatAttribute("x", &targetVector.x);
    int yQueryResult = xmlElement->QueryFloatAttribute("y", &targetVector.y);
    int zQueryResult = xmlElement->QueryFloatAttribute("z", &targetVector.z);
    if (xQueryResult == XML_NO_ATTRIBUTE){
      throwMandatoryAttributeException("<x>");
    }
    if (yQueryResult == XML_NO_ATTRIBUTE){
      throwMandatoryAttributeException("<y>");
    }
    if (zQueryResult == XML_NO_ATTRIBUTE){
      throwMandatoryAttributeException("<z>");
    }
  } else {
    throw runtime_error(invalidElementMessage);
  }
}

void XmlHelper::throwMandatoryAttributeException(const std::string &message){
  throw runtime_error(mandatoryAttributeMessage + message);
}

void XmlHelper::extractPosition(XMLElement *xmlElement, Vector3f &position) {
  pushAttributeVertexToVector(xmlElement->FirstChildElement("position"), position);
}

void XmlHelper::extractRotation(XMLElement *xmlElement, Vector3f &rotation) {
  XMLElement *elm = xmlElement->FirstChildElement("rotation");
  if (elm) {
    Vector3f tmpRot;
    pushAttributeVertexToVector(elm, tmpRot);
    rotation.pitch = rad(tmpRot.x);
    rotation.yaw   = rad(tmpRot.y);
    rotation.roll  = rad(tmpRot.z);
  } else {
    rotation.pitch = rotation.yaw = rotation.roll = 0;
  }
}

void XmlHelper::extractColor(XMLElement *xmlElement, Vector3f &color) {
  XMLElement *elm = xmlElement->FirstChildElement("color");
  if (elm) {
    elm->QueryFloatAttribute("r", &color.x);
    elm->QueryFloatAttribute("g", &color.y);
    elm->QueryFloatAttribute("b", &color.z);

  } else {
    color.x = color.y = color.z = 0;
  }
}


void XmlHelper::extractScale(XMLElement *xmlElement, Vector3f &scale) {
  pushAttributeVertexToVector(xmlElement->FirstChildElement("scale"), scale);
}

void XmlHelper::extractFileAttribute(tinyxml2::XMLElement *xmlElement, std::string &fileName) {
  const char* rawFileName = xmlElement->Attribute("file");
  if (rawFileName == nullptr) {
    throwMandatoryAttributeException("file");
  }

  fileName = rawFileName;
}

bool XmlHelper::extractBooleanAttribute(tinyxml2::XMLElement *xmlElement, std::string attribute, bool default) {
  if (xmlElement->Attribute(attribute)) {
    std::string attributeValue = xmlElement->Attribute(attribute);
    if (attributeValue == "true") {
      return true;
    }

    if (attributeValue == "false") {
      return false;
    }
  }

  return default;
}

} /* namespace radix */
