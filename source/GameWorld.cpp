#include <radix/GameWorld.hpp>

namespace radix {
GameWorld::GameWorld(InputSource &input) :
  input(input),
  splashScreen(radix::XmlScreenLoader::getScreen(radix::LegacyEnvironment::getDataDir() + "/screens/title.xml")),
  pauseScreen(radix::XmlScreenLoader::getScreen(radix::LegacyEnvironment::getDataDir() + "/screens/pause.xml")),
  gameOverScreen(radix::XmlScreenLoader::getScreen(radix::LegacyEnvironment::getDataDir() + "/screens/end.xml"))
{
}

void GameWorld::addScreen(Screen &screen) {
  screens.push_back(&screen);
}

void GameWorld::removeScreen(Screen &screen) {
  screens.erase(std::remove(screens.begin(), screens.end(), &screen), screens.end());
}

std::list<Screen*>* GameWorld::getScreens() {
  return &screens;
}
} /* namespace radix */
