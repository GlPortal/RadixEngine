#include <radix/data/map/MapTrigger.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/core/state/GameState.hpp>
#include <radix/env/Environment.hpp>

using namespace std;

namespace radix {

const std::string MapTrigger::TYPE = "map";

MapTrigger::MapTrigger(std::string filePath) {
  this->filePath = filePath;
}

void MapTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);
  std::string fileName = this->filePath;
  trigger.setActionOnEnter([fileName] (entities::Trigger &trigger) {
    XmlMapLoader mapLoader(trigger.world, trigger.world.game.getCustomTriggers());
    mapLoader.load(Environment::getDataDir() + "/maps/" + fileName + ".xml");
  });
}

} /* namespace radix */
