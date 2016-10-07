#include <radix/screen/XMLScreenLoader.hpp>
#include <radix/env/Util.hpp>
#include <radix/core/math/Vector3f.hpp>

using namespace tinyxml2;

namespace radix {

std::shared_ptr<Screen> XMLScreenLoader::loadScreen(const std::string &path) {
  std::shared_ptr<Screen> screen = std::make_shared<Screen>(); //setup screen pointer

  XMLDocument doc;
  XMLError error = doc.LoadFile(path.c_str()); //load in XML document

  if (error == 0){
    XMLHandle docHandle(&doc);
    XMLElement *element = docHandle.FirstChildElement("screen").ToElement();
    XMLHandle rootHandle = XMLHandle(element);

    //screen->textColor = loadTextColor(rootHandle);
    //screen->bgColor = loadbgColor(rootHandle);

    if (!loadText(rootHandle, &screen->text)) Util::Log(Error, "XMLScreenLoader") << "Failed to load text in " << path;
    //if (screen->textColor.x == 0) Util::Log(Error, "XMLScreenLoader") << "Failed to find text color element in " << path;
    //if (screen->bgColor.x == 0) Util::Log(Error, "XMLScreenLoader") << "Failed to find background color element in " << path;

    Util::Log(Debug, "XMLScreenLoader") << "Screen " << path << " loaded";

    return screen;
  } else {
    Util::Log(Error, "XMLScreenLoader") << "Failed to load screen " << path;
    return nullptr;
  }
}

bool XMLScreenLoader::loadText(XMLHandle &rootHandle, std::vector<Text>* text) {
  XMLElement *currElement = rootHandle.FirstChildElement("text").ToElement(); //grab the first element under the text section
  if (currElement){
    do {
      Text tempText{};

      currElement->QueryFloatAttribute("z", &tempText.z);
      currElement->QueryFloatAttribute("top", &tempText.top);
      currElement->QueryFloatAttribute("size", &tempText.size);
      tempText.align = currElement->Attribute("align");
      if (tempText.align != "centered" or tempText.align != "left" or tempText.align != "right")
        Util::Log(Error, "XMLScreenLoader") << "Alignment " << tempText.align << " is not supported!";
      tempText.text = currElement->GetText();

      Util::Log(Debug, "XMLScreenLoader") << tempText.align;

      text->push_back(tempText);
    } while((currElement = currElement->NextSiblingElement("text")) != nullptr);
  } else return false;

  return true;
}
/* Color will be done later
Vector4f XMLScreenLoader::loadTextColor(tinyxml2::XMLHandle &rootHandle) {
  Vector4f color;
  XMLElement *currElement = rootHandle.FirstChildElement("ColorSection").FirstChildElement("TextColor").ToElement();

  if (currElement) {
    currElement->QueryFloatAttribute("r", &color.x);
    currElement->QueryFloatAttribute("g", &color.y);
    currElement->QueryFloatAttribute("b", &color.z);
    currElement->QueryFloatAttribute("a", &color.w);
  }

  return color;
}

Vector4f XMLScreenLoader::loadbgColor(tinyxml2::XMLHandle &rootHandle) {
  Vector4f color;
  XMLElement *currElement = rootHandle.FirstChildElement("ColorSection").FirstChildElement("BgColor").ToElement();

  if (currElement) {
    currElement->QueryFloatAttribute("r", &color.x);
    currElement->QueryFloatAttribute("g", &color.y);
    currElement->QueryFloatAttribute("b", &color.z);
    currElement->QueryFloatAttribute("a", &color.w);
  }

  return color;
} */
}
