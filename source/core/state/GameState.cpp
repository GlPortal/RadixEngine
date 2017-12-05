#include <radix/core/state/GameState.hpp>

#include <radix/core/state/HandleGameFunction.hpp>
#include <radix/World.hpp>

namespace radix {

void GameState::handleInput(BaseGame &game) {
  HandleGameFunction stateFunction = game.getWorld()->stateFunctionStack.top();
  stateFunction(game);
}

} /* namespace radix */
