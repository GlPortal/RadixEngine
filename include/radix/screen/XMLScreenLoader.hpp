#ifndef RADIX_XMLSCREENLOADER_HPP
#define RADIX_XMLSCREENLOADER_HPP

#include <memory>
#include <tinyxml2.h>
#include "Screen.hpp"

namespace radix {

class XMLScreenLoader {
public:
  static std::shared_ptr<Screen> loadScreen(const std::string &path);
private:
  static bool loadText(tinyxml2::XMLHandle &rootHandle, std::vector<Text>* text);
  static bool extractColor(tinyxml2::XMLElement* currElement, Vector4f* color);
};
} /* namespace radix */

#endif //RADIX_XMLSCREENLOADER_HPP
