#include <radix/data/map/DeathTrigger.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/Health.hpp>

using namespace std;

namespace radix {

const std::string DeathTrigger::TYPE = "death";

void DeathTrigger::addAction(Entity& trigger) {
  std::function<void(BaseGame&)> action;
  action = [] (BaseGame &game) {
    game.getWorld()->getPlayer().getComponent<Health>().kill();
  };

  trigger.getComponent<Trigger>().setActionOnEnter(action);
}
} /* namespace radix */
