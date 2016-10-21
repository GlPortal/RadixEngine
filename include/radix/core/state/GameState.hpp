#ifndef RADIX_GAMESTATE_HPP
#define RADIX_GAMESTATE_HPP

#include <radix/BaseGame.hpp>

namespace radix {

class GameState {

public:
  void process(BaseGame &game);
};

} /* namespace radix */

#endif /* RADIX_GAMESTATE_HPP */
