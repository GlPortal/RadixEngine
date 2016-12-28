#include <radix/util/XmlLoader.hpp>
#include <radix/env/Util.hpp>

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

void XmlLoader::handleFailureForElement(std::string module, std::string element, std::string path) {
  Util::Log(Error, module) << "Failed to load element " << element << " in " << path;
}

} /* namespace radix */
