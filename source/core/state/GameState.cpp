#include <radix/core/state/GameState.hpp>

namespace radix {

void GameState::handleInput(BaseGame &game) {
  HandleGameFunction stateFunction = game.getWorld()->stateFunctionStack.top();
  stateFunction(game);
}

void GameState::handleRunning(BaseGame &game) { }

void GameState::handlePaused(BaseGame &game) { }

void GameState::handleSplash(BaseGame &game) { }

void GameState::handleMenu(BaseGame &game) { }

void GameState::handleGameOverScreen(BaseGame &game) { }

void GameState::handleWinScreen(BaseGame &game) { }

} /* namespace radix */
