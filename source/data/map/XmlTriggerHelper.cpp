#include <radix/data/map/XmlTriggerHelper.hpp>
#include <radix/data/map/WinTrigger.hpp>
#include <radix/data/map/DeathTrigger.hpp>
#include <radix/data/map/RadiationTrigger.hpp>
#include <radix/data/map/AudioTrigger.hpp>
#include <radix/data/map/MapTrigger.hpp>
#include <radix/data/map/XmlHelper.hpp>
#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/env/Environment.hpp>

using namespace std;
using namespace tinyxml2;

namespace radix {

void XmlTriggerHelper::extractTriggerActions(Entity& trigger, XMLElement *xmlElement) {
  std::function<void(BaseGame&)> action;
  std::string type = xmlElement->Attribute("type");
  trigger.addComponent<Trigger>();
  if (type == DeathTrigger::TYPE) {
    DeathTrigger deathTrigger = DeathTrigger();
    deathTrigger.addAction(trigger);
  } else if (type == WinTrigger::TYPE) {
    WinTrigger winTrigger = WinTrigger();
    winTrigger.addAction(trigger);
  } else if (type == RadiationTrigger::TYPE) {
    RadiationTrigger radiationTrigger = RadiationTrigger();
    radiationTrigger.addAction(trigger);
  } else if (type == AudioTrigger::TYPE) {
    bool loop = loop = XmlHelper::extractBooleanAttribute(xmlElement, "loop", false);

    std::string fileName;
    XmlHelper::extractFileAttribute(xmlElement, fileName);

    AudioTrigger audioTrigger = AudioTrigger(fileName);
    audioTrigger.setLoop(loop);
    audioTrigger.addAction(trigger);
  } else if (type == MapTrigger::TYPE) {
    std::string fileName;
    XmlHelper::extractFileAttribute(xmlElement, fileName);
    MapTrigger mapTrigger = MapTrigger(fileName);
    mapTrigger.addAction(trigger);
  } else if (type == "checkpoint") {
    XMLElement *spawnElement = xmlElement->FirstChildElement("spawn");
    action = [spawnElement] (BaseGame &game) {
      Vector3f position;
      Vector3f rotation;

      XmlHelper::extractPosition(spawnElement, position);
      XmlHelper::extractRotation(spawnElement, rotation);

      game.getWorld()->getPlayer().getComponent<Transform>().setPosition(position);
      game.getWorld()->getPlayer().getComponent<Transform>().setOrientation(Quaternion().fromAero(rotation));
    };

    trigger.getComponent<Trigger>().setActionOnEnter(action);
  }
}
} /* namespace radix */
