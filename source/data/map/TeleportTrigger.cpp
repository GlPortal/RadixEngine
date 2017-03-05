#include <radix/data/map/TeleportTrigger.hpp>

#include <radix/component/Trigger.hpp>

namespace radix {

const std::string TeleportTrigger::TYPE = "teleport";

TeleportTrigger::TeleportTrigger(std::string &destination) : destination(destination) {}

void TeleportTrigger::addAction(Entity &trigger) {
  std::function<void(BaseGame&)> action;

  action = [this] (BaseGame& game) {
    Transform& transform = game.getWorld()->getPlayer().getComponent<Transform>();
    transform.setPosition(game.getWorld()->destinations.at(destination).position);
    transform.setOrientation(Quaternion().fromAero(game.getWorld()->destinations.at(destination)
                                                     .rotation));
  };
}

} /* namespace radix */