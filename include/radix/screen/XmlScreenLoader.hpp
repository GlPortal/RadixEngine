#ifndef RADIX_XML_SCREEN_LOADER_HPP
#define RADIX_XML_SCREEN_LOADER_HPP

#include <memory>
#include <map>
#include <tinyxml2.h>

#include <radix/screen/Screen.hpp>
#include <radix/text/Text.hpp>

namespace radix {

class XmlScreenLoader {
public:
  static Screen& getScreen(const std::string& path);
private:
  static std::map<std::string, std::shared_ptr<Screen>> screenCache;
  static std::shared_ptr<Screen> loadScreen(const std::string& path);
  static bool loadText(tinyxml2::XMLHandle &rootHandle, std::vector<Text>* text);
  static bool extractColor(tinyxml2::XMLElement* currElement, Vector4f* color);
};
} /* namespace radix */

#endif //RADIX_XML_SCREEN_LOADER_HPP
