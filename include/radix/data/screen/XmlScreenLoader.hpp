#ifndef RADIX_XML_SCREEN_LOADER_HPP
#define RADIX_XML_SCREEN_LOADER_HPP

#include <memory>
#include <map>
#include <tinyxml2.h>

#include <radix/data/screen/Screen.hpp>
#include <radix/util/XmlLoader.hpp>
#include <radix/data/text/Text.hpp>

namespace radix {

class World;
  
class XmlScreenLoader : public XmlLoader {
public:
  static Screen& getScreen(World &world, const std::string& path);
private:
  static ScreenConstructor loadConstructor (const std::string& path);
  static std::shared_ptr<Screen> construct(World &world, const ScreenConstructor &constructor);
  static const std::string MODULE_NAME;
  static bool loadText(tinyxml2::XMLHandle &rootHandle, std::vector<Text>* text);
  static std::map<std::string, ScreenConstructor> screenCache;
};

} /* namespace radix */

#endif /* RADIX_XML_SCREEN_LOADER_HPP */
