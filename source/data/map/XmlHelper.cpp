#include <radix/data/map/XmlHelper.hpp>
#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/core/state/GameStateManager.hpp>
#include <radix/SoundManager.hpp>
#include <radix/env/LegacyEnvironment.hpp>

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
    float xQueryResult = xmlElement->QueryFloatAttribute("x", &targetVector.x);
    float yQueryResult = xmlElement->QueryFloatAttribute("y", &targetVector.y);
    float zQueryResult = xmlElement->QueryFloatAttribute("z", &targetVector.z);
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
    /*
     * By convention the vector representing the rotation angles is
     * ordered in the following way:
     * vector.x = angle around y
     * vector.y = angle around x
     * vector.z = angle around z
     *
     * But the blender plugin gives the angles in the following order:
     * vectorBlender.x = angle around x
     * vectorBlender.y = angle around y
     * vectorBlender.z = angle around z
     *
     * So some reordering is needed:
     */
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

bool XmlHelper::extractBooleanAttribute(tinyxml2::XMLElement *xmlElement, std::string attribute, bool defaultValue) {
  if (xmlElement->Attribute(attribute.c_str())) {
    std::string attributeValue = xmlElement->Attribute(attribute.c_str());
    if (attributeValue == "true") {
      return true;
    }

    if (attributeValue == "false") {
      return false;
    }
  }

  return defaultValue;
}

std::string XmlHelper::extractStringAttribute(tinyxml2::XMLElement *xmlElement, std::string attribute) {
  const char* rawString = xmlElement->Attribute(attribute.c_str());
  if (rawString == nullptr) {
    return "";
  }

  return std::string(rawString);
}

std::string XmlHelper::extractStringMandatoryAttribute(tinyxml2::XMLElement *xmlElement, std::string attribute) {
  const char* rawString = xmlElement->Attribute(attribute.c_str());
  if (rawString == nullptr) {
    throwMandatoryAttributeException(attribute);
  }

  return std::string(rawString);
}


} /* namespace radix */
