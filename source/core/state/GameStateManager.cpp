#include <radix/core/state/GameStateManager.hpp>

#include <radix/core/state/HandleGameFunction.hpp>
#include <radix/World.hpp>

namespace radix {

void GameStateManager::handleInput(BaseGame &game) {
  HandleGameFunction stateFunction = game.getWorld()->stateFunctionStack.top();
  stateFunction(game);
}

} /* namespace radix */
