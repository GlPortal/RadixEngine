#include <radix/data/map/TeleportTrigger.hpp>

#include <radix/component/Trigger.hpp>
#include <radix/component/Player.hpp>
#include <radix/system/PhysicsSystem.hpp>

namespace radix {

const std::string TeleportTrigger::TYPE = "teleport";

TeleportTrigger::TeleportTrigger(std::string destination) : destination(destination) {}

void TeleportTrigger::addAction(Entity &trigger) {
  std::function<void(BaseGame&)> action;
  std::string dest = destination;

  action = [dest] (BaseGame& game) {
    Transform& transform = game.getWorld()->getPlayer().getComponent<Transform>();
    KinematicCharacterController &controller = *game.getWorld()->getPlayer().getComponent<Player>()
      .controller;
    if (game.getWorld()->destinations.find(dest)
        != game.getWorld()->destinations.end()) {
      transform.setPosition(game.getWorld()->destinations.at(dest).position);
      transform.setOrientation(Quaternion().fromAero(game.getWorld()->destinations.at(dest)
                                                       .rotation));
    }
  };
  trigger.getComponent<Trigger>().setActionOnUpdate(action);
}

} /* namespace radix */