#include <radix/data/map/TeleportTrigger.hpp>

#include <radix/entities/Trigger.hpp>
#include <radix/BaseGame.hpp>
#include <radix/entities/Player.hpp>
#include <radix/simulation/Physics.hpp>
#include <radix/World.hpp>

namespace radix {

const std::string TeleportTrigger::TYPE = "teleport";

TeleportTrigger::TeleportTrigger(std::string destination) : destination(destination) {}

void TeleportTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);

  std::string dest = destination;

  trigger.setActionOnUpdate([dest] (entities::Trigger &trigger) {
    entities::Player &player = trigger.world.getPlayer();
    if (trigger.world.destinations.find(dest)
        != trigger.world.destinations.end()) {
      player.setPosition(trigger.world.destinations.at(dest).position);
      player.setOrientation(Quaternion().fromAero(trigger.world.destinations.at(dest)
                                                       .rotation));
      player.setHeadOrientation(Quaternion().fromAero(trigger.world.destinations.at(dest)
                                                        .rotation));
    }
  });
}

} /* namespace radix */
