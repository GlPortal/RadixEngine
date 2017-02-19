#include <radix/data/map/WinTrigger.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/Health.hpp>
#include <radix/core/state/GameState.hpp>

using namespace std;

namespace radix {

  const std::string WinTrigger::TYPE = "win";

  void WinTrigger::addAction(Entity& trigger) {
    std::function<void(BaseGame&)> action;
    action = [] (BaseGame &game) {
      game.getWorld()->event.dispatch(GameState::WinEvent());
    };

    trigger.getComponent<Trigger>().setActionOnUpdate(action);
  }
} /* namespace radix */
