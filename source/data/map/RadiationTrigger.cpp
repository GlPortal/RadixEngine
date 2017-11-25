#include <radix/data/map/RadiationTrigger.hpp>

#include <radix/entities/Player.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/core/state/GameState.hpp>
#include <radix/World.hpp>

using namespace std;

namespace radix {

const std::string RadiationTrigger::TYPE = "radiation";

void RadiationTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);
  trigger.setActionOnUpdate([] (entities::Trigger &trigger) {
    trigger.world.getPlayer().harm(0.1f);
  });
}

} /* namespace radix */
