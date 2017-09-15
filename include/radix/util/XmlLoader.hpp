#ifndef RADIX_UTIL_XML_LOADER_HPP
#define RADIX_UTIL_XML_LOADER_HPP

#include <string>

#include <tinyxml2.h>

#include <radix/core/math/Vector4f.hpp>

namespace radix {

class XmlLoader {
public:
  static std::string errorName(tinyxml2::XMLError err);

protected:
  static bool extractColor(tinyxml2::XMLElement* currElement, Vector4f* color);
  static void handleFailureForElement(std::string module, std::string element, std::string path);
};

} /* namespace radix */

#endif /* RADIX_UTIL_XML_LOADER_HPP */
