#include <radix/core/state/GameState.hpp>

namespace radix {

void GameState::handleInput(BaseGame &game) {
  HandleGameFunction stateFunction = game.getWorld()->stateFunctionStack.top();
  stateFunction(game);
}

} /* namespace radix */
