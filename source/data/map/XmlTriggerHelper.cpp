#include <radix/data/map/XmlTriggerHelper.hpp>
#include <radix/data/map/WinTrigger.hpp>
#include <radix/data/map/DeathTrigger.hpp>
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
  } else if (type == "radiation") {
    XmlTriggerHelper::addRadiationAction(trigger);
  } else if (type == "audio") {
    bool loop = false;
    if (xmlElement->Attribute("loop")) {
      std::string loopAttribute = xmlElement->Attribute("loop");
      if (loopAttribute == "true") {
        loop = true;
      }
    }

    std::string datadir = Environment::getDataDir();
    std::string fileName =
      datadir + "/audio/" + xmlElement->Attribute("file");
    action = [fileName] (BaseGame &game) {
      if (!SoundManager::isPlaying(fileName)) {
        SoundManager::playMusic(fileName);
      }
    };

    trigger.getComponent<Trigger>().setActionOnEnter(action);
  } else if (type == "map") {
    std::string filename = xmlElement->Attribute("file");
    addMapAction(filename, trigger);
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

void XmlTriggerHelper::addRadiationAction(Entity& trigger) {
  std::function<void(BaseGame&)> action;
  action = [] (BaseGame &game) {
    game.getWorld()
    ->getPlayer().getComponent<Health>().harm(0.1f);
  };
  trigger.getComponent<Trigger>().setActionOnUpdate(action);

}

void XmlTriggerHelper::addMapAction(std::string filename, Entity& trigger){
  std::function<void(BaseGame&)> action;
  action = [filename] (BaseGame &game) {
    XmlMapLoader mapLoader(*game.getWorld());
    mapLoader.load(Environment::getDataDir() + "maps/" + filename);
  };

  trigger.getComponent<Trigger>().setActionOnEnter(action);
}
} /* namespace radix */
