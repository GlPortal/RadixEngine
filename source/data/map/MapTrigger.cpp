#include <radix/data/map/MapTrigger.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/core/state/GameState.hpp>
#include <radix/env/Environment.hpp>

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
    XmlMapLoader mapLoader(newWorld, game.getCustomTriggers());
    mapLoader.load(Environment::getDataDir() + "/maps/" + fileName + ".xml");
    game.deferPostCycle([fileName, &game] () {
      game.switchToOtherWorld(fileName);
    });
  });
}

} /* namespace radix */
