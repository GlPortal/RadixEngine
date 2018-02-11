#ifndef RADIX_GAME_WORLD_HPP
#define RADIX_GAME_WORLD_HPP

#include <list>
#include <algorithm>

#include <radix/data/screen/XmlScreenLoader.hpp>
#include <radix/env/Environment.hpp>

namespace radix {

class InputSource;
struct Screen;

class GameWorld {
public:
  InputSource &input;
  radix::Screen &splashScreen;
  radix::Screen &pauseScreen;
  radix::Screen &gameOverScreen;
  GameWorld(InputSource &input);
  void addScreen(Screen& screen);
  void removeScreen(Screen& screen);
  std::list<Screen*>* getScreens();
private:
  std::list<Screen*> screens;
};

} /* namespace radix */

#endif /* RADIX_GAME_WORLD_HPP */
