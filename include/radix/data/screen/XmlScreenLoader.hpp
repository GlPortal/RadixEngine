#ifndef RADIX_XML_SCREEN_LOADER_HPP
#define RADIX_XML_SCREEN_LOADER_HPP

#include <memory>
#include <map>
#include <tinyxml2.h>
#include<stdexcept>

#include <radix/data/screen/Screen.hpp>
#include <radix/util/XmlLoader.hpp>
#include <radix/data/text/Text.hpp>

namespace radix {

class XmlScreenLoader : public XmlLoader {
public:
  static Screen& getScreen(const std::string& path);
private:
  static std::map<std::string, std::shared_ptr<Screen>> screenCache;
  static std::shared_ptr<Screen> loadScreen(const std::string& path);
  static const std::string MODULE_NAME;
  static bool loadText(tinyxml2::XMLHandle &rootHandle, std::vector<Text>* text);
};

} /* namespace radix */

#endif /* RADIX_XML_SCREEN_LOADER_HPP */
