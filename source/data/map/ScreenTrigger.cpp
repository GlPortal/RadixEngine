#include <radix/data/map/ScreenTrigger.hpp>

#include <radix/BaseGame.hpp>
#include <radix/data/screen/XmlScreenLoader.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/env/Environment.hpp>

namespace radix {
  
const std::string ScreenTrigger::TYPE = "screen";

ScreenTrigger::ScreenTrigger(const std::string &file) : file(file) { }

void ScreenTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);
  std::string fileName = file;
  trigger.setActionOnEnter([fileName] (entities::Trigger &trigger) {
    Screen &screen = XmlScreenLoader::getScreen(Environment::getDataDir() + "/screens/"
        + fileName + ".xml");
    trigger.world.game.getGameWorld()->addScreen(screen);
  });
}
  
} /* namespace radix */
