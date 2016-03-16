#ifndef RADIX_XMLHELPER_HPP
#define RADIX_XMLHELPER_HPP

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
  static void extractRotation(tinyxml2::XMLElement *xmlElement, Vector3f &rotation);
  static void extractScale(tinyxml2::XMLElement *xmlElement, Vector3f &scale);
};

} /* namespace radix */

#endif /* RADIX_XMLHELPER_HPP */
