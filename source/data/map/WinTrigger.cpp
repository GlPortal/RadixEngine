#include <radix/data/map/WinTrigger.hpp>

#include <radix/entities/Trigger.hpp>
#include <radix/core/state/GameStateManager.hpp>
#include <radix/World.hpp>

using namespace std;

namespace radix {

const std::string WinTrigger::TYPE = "win";

void WinTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);
  trigger.setActionOnUpdate([] (entities::Trigger &trigger) {
    trigger.world.event.dispatch(GameStateManager::WinEvent());
  });
}

} /* namespace radix */
