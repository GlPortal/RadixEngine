#include <radix/data/map/MapTrigger.hpp>

#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/core/state/GameStateManager.hpp>
#include <radix/env/LegacyEnvironment.hpp>
#include <radix/World.hpp>

using namespace std;

namespace radix {

const std::string MapTrigger::TYPE = "map";

MapTrigger::MapTrigger(const std::string &filePath) :
  filePath(filePath) {
}

void MapTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);
  std::string fileName = filePath;
  trigger.setActionOnEnter([fileName] (entities::Trigger &trigger) {
    BaseGame &game = trigger.world.game;
    World &newWorld = game.createOtherWorld<World>(fileName);
    game.deferPostCycle([fileName, &game, &newWorld] () {
      XmlMapLoader mapLoader(newWorld, game.getCustomTriggers());
      mapLoader.load(LegacyEnvironment::getDataDir() + "/maps/" + fileName + ".xml");
      game.switchToOtherWorld(fileName);
    });
  });
}

} /* namespace radix */
