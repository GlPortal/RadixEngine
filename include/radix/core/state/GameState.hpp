//
// Created by geert on 9/25/16.
//

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class Game;

namespace radix {

class GameState {

public:
  void handleInput(Game &game);
  static void handleRunning(Game &game);
  static void handlePaused(Game &game);
  static void handleSplash(Game &game);
  static void handleMenu(Game &game);
  static void handleGameOverScreen(Game &game);
  static void handleWinScreen(Game &game);
};
} /* namespace radix */

#endif //GAMESTATE_HPP
