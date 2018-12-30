#include <radix/data/screen/XmlScreenLoader.hpp>
#include <radix/env/Util.hpp>
#include <ciso646>

using namespace tinyxml2;

namespace radix {

std::map<std::string, std::shared_ptr<Screen>> XmlScreenLoader::screenCache = { };
const std::string XmlScreenLoader::MODULE_NAME = "XmlScreenLoader";

Screen& XmlScreenLoader::getScreen(const std::string &path) {
  auto it = screenCache.find(path);
  if (it != screenCache.end()) {
    return *it->second;
  }

  std::shared_ptr<Screen> screen = loadScreen(path);
  screenCache.insert(std::make_pair(path, screen));
  return *screen;
}

std::shared_ptr<Screen> XmlScreenLoader::loadScreen(const std::string &path) {
  XMLDocument doc(true, COLLAPSE_WHITESPACE);
  XMLError error = doc.LoadFile(path.c_str());
  const std::string &module = XmlScreenLoader::MODULE_NAME;

  if (error == 0) {
    std::shared_ptr<Screen> screen = std::make_shared<Screen>(); //setup screen pointer
    XMLHandle docHandle(&doc);
    XMLElement *element  = docHandle.FirstChildElement("screen").ToElement();
    XMLHandle rootHandle = XMLHandle(element);

    if (not loadText(rootHandle, &screen->text)) {
      XmlScreenLoader::handleFailureForElement(module, std::string("text"), path);
    }
    if (not extractColor(element, &screen->color)) {
      XmlScreenLoader::handleFailureForElement(module, std::string("color"), path);
    }

    Util::Log(Debug, module) << "Screen " << path << " loaded";

    return screen;
  }
  throw std::runtime_error("Failed to load " + path + ": " + errorName(error));
}

bool XmlScreenLoader::loadText(XMLHandle &rootHandle, std::vector<Text>* textVector) {
  //grab the first element under the text section
  XMLElement *currentElement = rootHandle.FirstChildElement("text").ToElement();
  if (currentElement) {
    do {
      Text text{};
      std::string align;

      currentElement->QueryFloatAttribute("z", &text.z);
      currentElement->QueryFloatAttribute("top", &text.top);
      currentElement->QueryFloatAttribute("size", &text.size);
      if (not extractColor(currentElement, &text.color)) {
        return false;
      }
      align = currentElement->Attribute("align");

      if (align == "center") {
        text.align = Text::Center;
      } else if (align == "left") {
        text.align = Text::Left;
      } else if (align == "right") {
        text.align = Text::Right;
      } else {
        Util::Log(Error, XmlScreenLoader::MODULE_NAME) << "Alignment \"" << align << "\" is not supported!";
        continue;
      }

      text.content = currentElement->GetText();

      textVector->push_back(text);
    } while ((currentElement = currentElement->NextSiblingElement("text")) != nullptr);
  } else {
    return false;
  }

  return true;
}
} /* namespace radix */
