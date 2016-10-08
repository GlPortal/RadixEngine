#include <radix/core/state/GameState.hpp>

namespace radix {

void GameState::process(BaseGame &game) {
  HandleGameFunction stateFunction = game.getWorld()->stateFunctionStack.top();
  stateFunction(game);
}

} /* namespace radix */
