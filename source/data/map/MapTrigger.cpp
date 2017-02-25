#include <radix/data/map/MapTrigger.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/core/state/GameState.hpp>
#include <radix/env/Environment.hpp>

using namespace std;

namespace radix {

  const std::string MapTrigger::TYPE = "map";

  MapTrigger::MapTrigger(std::string filePath) {
    this->filePath = filePath;
  }

  void MapTrigger::addAction(Entity& trigger) {
    std::function<void(BaseGame&)> action;
    std::string fileName = this->filePath;
    action = [fileName] (BaseGame &game) {
      XmlMapLoader mapLoader(*game.getWorld());
      mapLoader.load(Environment::getDataDir() + "maps/" + fileName);
    };

    trigger.getComponent<Trigger>().setActionOnEnter(action);
  }
} /* namespace radix */
