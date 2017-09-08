#include <radix/data/map/DeathTrigger.hpp>

#include <radix/entities/Trigger.hpp>
#include <radix/BaseGame.hpp>

using namespace std;

namespace radix {

const std::string DeathTrigger::TYPE = "death";

void DeathTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);
  trigger.setActionOnEnter([] (entities::Trigger &trigger) {
    trigger.world.getPlayer().kill();
  });
}

} /* namespace radix */
