#include <radix/util/XmlLoader.hpp>

using namespace tinyxml2;

namespace radix {

bool XmlLoader::extractColor(XMLElement* currentElement, Vector4f* color) {
  if (currentElement) {
    currentElement->QueryFloatAttribute("r", &color->x);
    currentElement->QueryFloatAttribute("g", &color->y);
    currentElement->QueryFloatAttribute("b", &color->z);
    currentElement->QueryFloatAttribute("a", &color->w);
    return true;
  }
  return false;
}

} /* namespace radix */
