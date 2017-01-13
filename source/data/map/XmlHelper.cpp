#include <radix/data/map/XmlHelper.hpp>
#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/Health.hpp>
#include <radix/core/state/GameState.hpp>
#include <radix/SoundManager.hpp>
#include <radix/env/Environment.hpp>

using namespace std;
using namespace tinyxml2;

namespace radix {

std::string XmlHelper::mandatoryAttributeMessage("Mandatory attribute has not been defined.");
std::string XmlHelper::invalidElementMessage("pushAttributeToVector received an invalid "
                                                 "XML-Element.");
/**
  * Pushes vector coordinates from an XML-element to a Vector3f.
  * Trying to pull attributes from a non-existing element is considered an exception.
  */
void XmlHelper::pushAttributeVertexToVector(XMLElement *xmlElement, Vector3f &targetVector) {
  if (xmlElement) {
    int xQueryResult = xmlElement->QueryFloatAttribute("x", &targetVector.x);
    int yQueryResult = xmlElement->QueryFloatAttribute("y", &targetVector.y);
    int zQueryResult = xmlElement->QueryFloatAttribute("z", &targetVector.z);
    if (xQueryResult == XML_NO_ATTRIBUTE){
      throwMandatoryAttributeException("<x>");
    }
    if (yQueryResult == XML_NO_ATTRIBUTE){
      throwMandatoryAttributeException("<y>");
    }
    if (zQueryResult == XML_NO_ATTRIBUTE){
      throwMandatoryAttributeException("<z>");
    }
  } else {
    throw runtime_error(invalidElementMessage);
  }
}

void XmlHelper::throwMandatoryAttributeException(const std::string &message){
  throw runtime_error(mandatoryAttributeMessage + message);
}

void XmlHelper::extractPosition(XMLElement *xmlElement, Vector3f &position) {
  pushAttributeVertexToVector(xmlElement->FirstChildElement("position"), position);
}

void XmlHelper::extractRotation(XMLElement *xmlElement, Vector3f &rotation) {
  XMLElement *elm = xmlElement->FirstChildElement("rotation");
  if (elm) {
    Vector3f tmpRot;
    pushAttributeVertexToVector(elm, tmpRot);
    rotation.pitch = rad(tmpRot.x);
    rotation.yaw   = rad(tmpRot.y);
    rotation.roll  = rad(tmpRot.z);
  } else {
    rotation.pitch = rotation.yaw = rotation.roll = 0;
  }
}

void XmlHelper::extractColor(XMLElement *xmlElement, Vector3f &color) {
  XMLElement *elm = xmlElement->FirstChildElement("color");
  if (elm) {
    elm->QueryFloatAttribute("r", &color.x);
    elm->QueryFloatAttribute("g", &color.y);
    elm->QueryFloatAttribute("b", &color.z);

  } else {
    color.x = color.y = color.z = 0;
  }
}


void XmlHelper::extractScale(XMLElement *xmlElement, Vector3f &scale) {
  pushAttributeVertexToVector(xmlElement->FirstChildElement("scale"), scale);
}

void XmlHelper::extractTriggerActions(Entity& trigger, XMLElement *xmlElement) {
  std::string type = xmlElement->Attribute("type");
  if (type == "death") {
    trigger.addComponent<Trigger>();
    trigger.getComponent<Trigger>().setActionOnEnter
      (
       [] (BaseGame &game) {
         Util::Log(Debug, "XmlHelper") << "Death.";
         game.getWorld()->getPlayer();//.getComponent<Health>().kill();
       }
       );
  } else if (type == "win") {
    trigger.addComponent<Trigger>();
    trigger.getComponent<Trigger>().setActionOnUpdate
      (
       [] (BaseGame &game) {
      game.getWorld()->event.dispatch(GameState::WinEvent());

      });
  } else if (type == "radiation") {
    trigger.addComponent<Trigger>();
    trigger.getComponent<Trigger>().setActionOnUpdate
      (
       [] (BaseGame &game) {
         game.getWorld()
           ->getPlayer().getComponent<Health>().harm(0.1f);
       }
       );
  } else if (type == "music") {
   /*  bool loop = false;
    if (xmlElement->Attribute("loop") == "true") {
      loop = true;
    }
    looping music isn't supported */
    std::string track = xmlElement->Attribute("track");
    trigger.addComponent<Trigger>();
    trigger.getComponent<Trigger>().setActionOnEnter
      (
       [track] (BaseGame &game) {
         SoundManager::playMusic(track);
       }
       );
  } else if (type == "map") {
    std::string mapName = xmlElement->Attribute("map");
    trigger.addComponent<Trigger>();
    trigger.getComponent<Trigger>().setActionOnEnter
      (
       [mapName] (BaseGame &game) {

         XmlMapLoader mapLoader(*game.getWorld());
         mapLoader.load(Environment::getDataDir() + "maps/" + mapName);
       }
       );

  } else if (type == "checkpoint") {
    XMLElement *spawnElement = xmlElement->FirstChildElement("spawn");
    trigger.addComponent<Trigger>();
    trigger.getComponent<Trigger>().setActionOnEnter
      (
       [spawnElement] (BaseGame &game) {
         Vector3f position;
         Vector3f rotation;

         extractPosition(spawnElement, position);
         extractRotation(spawnElement, rotation);

         game.getWorld()->getPlayer().getComponent<Transform>().setPosition(position);
         game.getWorld()->getPlayer().getComponent<Transform>().setOrientation(Quaternion().fromAero(rotation));
       }
       );
  }
}

} /* namespace radix */
