#ifndef RADIX_XML_HELPER_HPP
#define RADIX_XML_HELPER_HPP

#include <string>

#include <tinyxml2.h>

#include <radix/core/math/Vector3f.hpp>
#include <radix/Entity.hpp>

namespace radix {

class XmlHelper {
public:
  static void pushAttributeVertexToVector(tinyxml2::XMLElement *xmlElement,
    Vector3f &targetVector);
  static std::string mandatoryAttributeMessage;
  static std::string invalidElementMessage;
  static void throwMandatoryAttributeException(const std::string &message);
  static void extractPosition(tinyxml2::XMLElement *xmlElement, Vector3f &position);
  static void extractColor(tinyxml2::XMLElement *xmlElement, Vector3f &color);
  static void extractRotation(tinyxml2::XMLElement *xmlElement, Vector3f &rotation);
  static void extractScale(tinyxml2::XMLElement *xmlElement, Vector3f &scale);
};

} /* namespace radix */

#endif /* RADIX_XML_HELPER_HPP */
