#ifndef RADIX_GAME_WORLD_HPP
#define RADIX_GAME_WORLD_HPP
#include <radix/input/InputSource.hpp>

namespace radix {

class GameWorld {
public:
  InputSource &input;
  GameWorld(InputSource &input);
  void showScreen();
  void hideScreen();
  bool isScreenVisible();
private:
  bool screenVisible;
};
} /* namespace radix */

#endif /* RADIX_GAME_WORLD_HPP */
