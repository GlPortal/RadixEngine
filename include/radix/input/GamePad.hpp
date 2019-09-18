#ifndef RADIX_GAME_PAD_HPP
#define RADIX_GAME_PAD_HPP

#include <SDL2/SDL.h>
#include <radix/env/Util.hpp>

namespace radix {

class GamePad {
public:
  GamePad();
  void create();
  SDL_GameController& getController();
private:
  SDL_GameController *controller;
  SDL_Joystick *joystick;
};

}

#endif /* RADIX_GAME_PAD_HPP */
