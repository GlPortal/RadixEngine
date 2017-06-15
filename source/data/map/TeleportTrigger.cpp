#include <radix/data/map/TeleportTrigger.hpp>

#include <radix/entities/Trigger.hpp>
#include <radix/entities/Player.hpp>
#include <radix/simulation/Physics.hpp>

namespace radix {

const std::string TeleportTrigger::TYPE = "teleport";

TeleportTrigger::TeleportTrigger(std::string destination) : destination(destination) {}

void TeleportTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);

  std::string dest = destination;

  trigger.setActionOnUpdate([dest] (BaseGame& game) {
    entities::Player &player = game.getWorld()->getPlayer();
    if (game.getWorld()->destinations.find(dest)
        != game.getWorld()->destinations.end()) {
      player.setPosition(game.getWorld()->destinations.at(dest).position);
      player.setOrientation(Quaternion().fromAero(game.getWorld()->destinations.at(dest)
                                                       .rotation));
      player.setHeadOrientation(Quaternion().fromAero(game.getWorld()->destinations.at(dest)
                                                        .rotation));
    }
  });
}

} /* namespace radix */
