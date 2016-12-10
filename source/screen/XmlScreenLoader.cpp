#include <radix/screen/XmlScreenLoader.hpp>
#include <radix/env/Util.hpp>

using namespace tinyxml2;

namespace radix {

std::map<std::string, std::shared_ptr<Screen>> XmlScreenLoader::screenCache = { };

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
  std::shared_ptr<Screen> screen = std::make_shared<Screen>(); //setup screen pointer

  XMLDocument doc(true, COLLAPSE_WHITESPACE);
  XMLError error = doc.LoadFile(path.c_str()); //load in XML document

  if (error == 0) {
    XMLHandle docHandle(&doc);
    XMLElement *element = docHandle.FirstChildElement("screen").ToElement();
    XMLHandle rootHandle = XMLHandle(element);

    if (not loadText(rootHandle, &screen->text)) {
      Util::Log(Error, "XmlScreenLoader") << "Failed to load text in " << path;
    }
    if (not extractColor(element, &screen->color)) {
      Util::Log(Error, "XmlScreenLoader") << "Failed to load color in " << path;
    }

    Util::Log(Debug, "XmlScreenLoader") << "Screen " << path << " loaded";

    return screen;
  } else {
    Util::Log(Error, "XmlScreenLoader") << "Failed to load screen " << path;
    return nullptr;
  }
}

bool XmlScreenLoader::loadText(XMLHandle &rootHandle, std::vector<Text>* text) {
  //grab the first element under the text section
  XMLElement *currElement = rootHandle.FirstChildElement("text").ToElement();
  if (currElement) {
    do {
      Text tempText{};
      std::string align;

      currElement->QueryFloatAttribute("z", &tempText.z);
      currElement->QueryFloatAttribute("top", &tempText.top);
      currElement->QueryFloatAttribute("size", &tempText.size);
      if (not extractColor(currElement, &tempText.color)) {
        return false;
      }
      align = currElement->Attribute("align");

      if (align == "center") {
        tempText.align = Text::Center;
      } else if (align == "left") {
        tempText.align = Text::Left;
      } else if (align == "right") {
        tempText.align = Text::Right;
      } else {
        Util::Log(Error, "XmlScreenLoader") << "Alignment \"" << align << "\" is not supported!";
        continue;
      }

      Util::Log(Debug, "XmlScreenLoader") << currElement->GetText();

      tempText.content = currElement->GetText();

      text->push_back(tempText);
    } while ((currElement = currElement->NextSiblingElement("text")) != nullptr);
  } else {
    return false;
  }

  return true;
}

bool XmlScreenLoader::extractColor(XMLElement* currElement, Vector4f* color) {
  if (currElement) {
    currElement->QueryFloatAttribute("r", &color->x);
    currElement->QueryFloatAttribute("g", &color->y);
    currElement->QueryFloatAttribute("b", &color->z);
    currElement->QueryFloatAttribute("a", &color->w);
    return true;
  }
  return false;
}

} /* namespace radix */
