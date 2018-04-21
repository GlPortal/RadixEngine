#include <radix/data/map/XmlTriggerHelper.hpp>
#include <radix/data/map/WinTrigger.hpp>
#include <radix/data/map/DeathTrigger.hpp>
#include <radix/data/map/RadiationTrigger.hpp>
#include <radix/data/map/AudioTrigger.hpp>
#include <radix/data/map/MapTrigger.hpp>
#include <radix/data/map/XmlHelper.hpp>
#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/env/Environment.hpp>
#include <radix/data/map/TeleportTrigger.hpp>
#include <radix/data/map/ScriptTrigger.hpp>

using namespace std;
using namespace tinyxml2;

namespace radix {

void XmlTriggerHelper::extractTriggerActions(entities::Trigger &trigger, XMLElement *xmlElement,
                                             const std::list<CustomTrigger> &customTriggers) {
  std::function<void(BaseGame&)> action;
  std::string type = xmlElement->Attribute("type");

  /* first go through custom triggers */
  auto it = customTriggers.begin();
  while(it != customTriggers.end()) {
    if (type == it->TYPE) {
      it->loadFunction(trigger, xmlElement);
      return;
    }
    it++;
  }

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
  } else if (type == TeleportTrigger::TYPE) {
    extractDestinationTriggerActions(trigger, xmlElement);
  } else if (type == ScriptTrigger::TYPE) {
    extractScriptTriggerActions(trigger, xmlElement);
  } else {
    XmlHelper::throwMandatoryAttributeException("trigger type");
  }
}

void XmlTriggerHelper::extractMapTriggerActions(entities::Trigger &trigger, XMLElement *xmlElement) {
  std::string fileName;
  XmlHelper::extractFileAttribute(xmlElement, fileName);
  MapTrigger mapTrigger = MapTrigger(fileName);
  mapTrigger.addAction(trigger);
}

void XmlTriggerHelper::extractAudioTriggerActions(entities::Trigger &trigger, XMLElement *xmlElement) {
  bool loop = XmlHelper::extractBooleanAttribute(xmlElement, "loop", false);

  std::string fileName;
  XmlHelper::extractFileAttribute(xmlElement, fileName);

  AudioTrigger audioTrigger = AudioTrigger(fileName);
  audioTrigger.setLoop(loop);
  audioTrigger.addAction(trigger);
}

void XmlTriggerHelper::extractDestinationTriggerActions(entities::Trigger &trigger, XMLElement *xmlElement) {
  std::string destination = XmlHelper::extractStringMandatoryAttribute(xmlElement, "destination");
  TeleportTrigger teleportTrigger = TeleportTrigger(destination);
  teleportTrigger.addAction(trigger);
}

void XmlTriggerHelper::extractScriptTriggerActions(entities::Trigger &trigger, XMLElement *xmlElement) {
  std::string code = xmlElement->FirstChildElement("code")->GetText();
  ScriptTrigger scriptTrigger = ScriptTrigger(code);
  scriptTrigger.addAction(trigger);
}
} /* namespace radix */
