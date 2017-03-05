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
    extractAudioTriggerActions(trigger, xmlElement);
  } else if (type == MapTrigger::TYPE) {
    extractMapTriggerActions(trigger, xmlElement);
  } else {
    XmlHelper::throwMandatoryAttributeException("trigger type");
  }
}

void XmlTriggerHelper::extractMapTriggerActions(Entity& trigger, XMLElement *xmlElement) {
  std::string fileName;
  XmlHelper::extractFileAttribute(xmlElement, fileName);
  MapTrigger mapTrigger = MapTrigger(fileName);
  mapTrigger.addAction(trigger);
}

void XmlTriggerHelper::extractAudioTriggerActions(Entity& trigger, XMLElement *xmlElement) {
  bool loop = XmlHelper::extractBooleanAttribute(xmlElement, "loop", false);

  std::string fileName;
  XmlHelper::extractFileAttribute(xmlElement, fileName);

  AudioTrigger audioTrigger = AudioTrigger(fileName);
  audioTrigger.setLoop(loop);
  audioTrigger.addAction(trigger);
}
} /* namespace radix */
