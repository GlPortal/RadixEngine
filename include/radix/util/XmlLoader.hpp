#ifndef RADIX_XML_LOADER_HPP
#define RADIX_XML_LOADER_HPP

#include <tinyxml2.h>
#include <radix/core/math/Vector4f.hpp>

namespace radix {

  class XmlLoader {
  protected:
    static bool extractColor(tinyxml2::XMLElement* currElement, Vector4f* color);
  };

} /* namespace radix */

#endif /* RADIX_XML_LOADER_HPP */
