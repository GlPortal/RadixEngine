//
// Created by geert on 9/25/16.
//

#ifndef RADIX_GAMESTATE_HPP
#define RADIX_GAMESTATE_HPP

#include <radix/BaseGame.hpp>

namespace radix {

class GameState {

public:
  void handleInput(BaseGame &game);
  static void handleRunning(BaseGame &game);
  static void handlePaused(BaseGame &game);
  static void handleSplash(BaseGame &game);
  static void handleMenu(BaseGame &game);
  static void handleGameOverScreen(BaseGame &game);
  static void handleWinScreen(BaseGame &game);
};

} /* namespace radix */

#endif /* RADIX_GAMESTATE_HPP */
