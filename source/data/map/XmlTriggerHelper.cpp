#include <radix/data/map/XmlTriggerHelper.hpp>
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
  std::string type = xmlElement->Attribute("type");
  trigger.addComponent<Trigger>();
  if (type == "death") {
    trigger.getComponent<Trigger>().setActionOnEnter
      (
       [] (BaseGame &game) {
         Util::Log(Debug, "XmlHelper") << "Death.";
         game.getWorld()->getPlayer();//.getComponent<Health>().kill();
       }
       );
  } else if (type == "win") {
    trigger.getComponent<Trigger>().setActionOnUpdate
      (
       [] (BaseGame &game) {
      game.getWorld()->event.dispatch(GameState::WinEvent());

      });
  } else if (type == "radiation") {
    trigger.getComponent<Trigger>().setActionOnUpdate
      (
       [] (BaseGame &game) {
         game.getWorld()
           ->getPlayer().getComponent<Health>().harm(0.1f);
       }
       );
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
    std::function<void(BaseGame&)> action = [fileName] (BaseGame &game) {
      if (!SoundManager::isPlaying(fileName)) {
        SoundManager::playMusic(fileName);
      }
    };
    trigger.getComponent<Trigger>().setActionOnEnter(action);
  } else if (type == "map") {
    std::string mapName = xmlElement->Attribute("file");
    trigger.getComponent<Trigger>().setActionOnEnter
      (
       [mapName] (BaseGame &game) {

         XmlMapLoader mapLoader(*game.getWorld());
         mapLoader.load(Environment::getDataDir() + "maps/" + mapName);
       }
       );

  } else if (type == "checkpoint") {
    XMLElement *spawnElement = xmlElement->FirstChildElement("spawn");
    trigger.getComponent<Trigger>().setActionOnEnter
      (
       [spawnElement] (BaseGame &game) {
         Vector3f position;
         Vector3f rotation;

         XmlHelper::extractPosition(spawnElement, position);
         XmlHelper::extractRotation(spawnElement, rotation);

         game.getWorld()->getPlayer().getComponent<Transform>().setPosition(position);
         game.getWorld()->getPlayer().getComponent<Transform>().setOrientation(Quaternion().fromAero(rotation));
       }
       );
  }
}

} /* namespace radix */
