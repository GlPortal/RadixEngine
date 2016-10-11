#include <radix/GameWorld.hpp>

namespace radix {
GameWorld::GameWorld(InputSource &input) :
  input(input),
  screenVisible(true){
}

void GameWorld::showScreen() {
  screenVisible = true;
}

void GameWorld::hideScreen() {
  screenVisible = false;
}

bool GameWorld::isScreenVisible() {
  return screenVisible;
}
} /* namespace radix */
