#include <radix/data/map/RadiationTrigger.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/Health.hpp>
#include <radix/core/state/GameState.hpp>

using namespace std;

namespace radix {

  const std::string RadiationTrigger::TYPE = "radiation";

  void RadiationTrigger::addAction(Entity& trigger) {
    std::function<void(BaseGame&)> action;
    action = [] (BaseGame &game) {
      game.getWorld()
      ->getPlayer().getComponent<Health>().harm(0.1f);
    };
    trigger.getComponent<Trigger>().setActionOnUpdate(action);
  }
} /* namespace radix */
