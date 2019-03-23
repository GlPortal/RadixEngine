#include <radix/input/GamePad.hpp>

namespace radix {
GamePad::GamePad() :controller() {
}

void GamePad::create(){
  int numJoysticks = SDL_NumJoysticks();
  Util::Log(Verbose, "Window") << "Number of game pads " << std::to_string(numJoysticks);

  for (int i = 0; i < numJoysticks; ++i) {
    if (SDL_IsGameController(i)) {
      controller = SDL_GameControllerOpen(i);
      if (controller) {
        Util::Log(Warning, "Window") << "Game pad of index " << i << " loaded";
        joystick = SDL_JoystickOpen(i);

        break;
      } else {
        Util::Log(Warning, "Window") << "Game pad of index " << i << " unable to load";
      }
    } else {
      Util::Log(Warning, "Window") << "Game pad of index " << i << " not a controller";
    }
  }
}

SDL_GameController &GamePad::getController() {
  return *(this->controller);
}

} /* namespace radix */
