#include <radix/data/map/WinTrigger.hpp>

#include <radix/entities/Trigger.hpp>
#include <radix/core/state/GameState.hpp>

using namespace std;

namespace radix {

const std::string WinTrigger::TYPE = "win";

void WinTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);
  trigger.setActionOnUpdate([] (BaseGame &game) {
    game.getWorld()->event.dispatch(GameState::WinEvent());
  });
}

} /* namespace radix */
