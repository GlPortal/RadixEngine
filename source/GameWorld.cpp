#include <radix/GameWorld.hpp>

namespace radix {
GameWorld::GameWorld(InputSource &input) :
  input(input){
}

void GameWorld::addScreen(Screen &screen) {
  screens.push_back(std::move(screen));
}

std::list<Screen>* GameWorld::getScreens()
{
  return &screens;
}
} /* namespace radix */
