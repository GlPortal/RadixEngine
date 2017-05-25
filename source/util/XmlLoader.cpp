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

static const char *errorNames[] = {
  "XML_SUCCESS",
  "XML_NO_ATTRIBUTE",
  "XML_WRONG_ATTRIBUTE_TYPE",
  "XML_ERROR_FILE_NOT_FOUND",
  "XML_ERROR_FILE_COULD_NOT_BE_OPENED",
  "XML_ERROR_FILE_READ_ERROR",
  "XML_ERROR_ELEMENT_MISMATCH",
  "XML_ERROR_PARSING_ELEMENT",
  "XML_ERROR_PARSING_ATTRIBUTE",
  "XML_ERROR_IDENTIFYING_TAG",
  "XML_ERROR_PARSING_TEXT",
  "XML_ERROR_PARSING_CDATA",
  "XML_ERROR_PARSING_COMMENT",
  "XML_ERROR_PARSING_DECLARATION",
  "XML_ERROR_PARSING_UNKNOWN",
  "XML_ERROR_EMPTY_DOCUMENT",
  "XML_ERROR_MISMATCHED_ELEMENT",
  "XML_ERROR_PARSING",
  "XML_CAN_NOT_CONVERT_TEXT",
  "XML_NO_TEXT_NODE"
};

std::string XmlLoader::errorName(XMLError err) {
  return errorNames[err];
}

} /* namespace radix */
