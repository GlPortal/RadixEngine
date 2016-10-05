#include <radix/screen/XMLScreenLoader.hpp>
#include <radix/env/Util.hpp>

using namespace tinyxml2;

namespace radix {

std::shared_ptr<Screen> XMLScreenLoader::loadScreen(const std::string &path) {
  std::shared_ptr<Screen> screen = std::make_shared<Screen>(); //setup screen pointer

  XMLDocument doc;
  XMLError error = doc.LoadFile(path.c_str()); //load in XML document

  if (error){
    XMLHandle docHandle(&doc);
    XMLElement *element = docHandle.FirstChildElement().ToElement();
    XMLHandle rootHandle = XMLHandle(element);

    screen->text = loadText(rootHandle);

    return screen;
  } else {
    Util::Log(Error, "XMLScreenLoader") << "Failed to load screen " << path;
    return nullptr;
  }
}

std::vector<Text> XMLScreenLoader::loadText(XMLHandle &rootHandle) {
  std::vector<Text> text;
  XMLElement *currElement = rootHandle.FirstChildElement("TextSection").FirstChildElement().ToElement(); //grab the first element under the text section
  if (currElement){
    do {
      Text tempText{};

      currElement->QueryIntAttribute("x", &tempText.x);
      currElement->QueryIntAttribute("y", &tempText.y);
      currElement->QueryFloatAttribute("size", &tempText.size);
      tempText.text = currElement->GetText();

      text.push_back(tempText);
    } while((currElement = currElement->NextSiblingElement()) != nullptr);
  }

  return text;
}
}
