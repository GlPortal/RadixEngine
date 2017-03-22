#ifndef RADIX_GAME_CONSOLE_HPP
#define RADIX_GAME_CONSOLE_HPP

#include <string>
#include <radix/env/Config.hpp>
#include <radix/BaseGame.hpp>

namespace radix {

  class GameConsole {
  public:
    static void run(BaseGame &game);
  };

} /* namespace radix */

#endif /* RADIX_GAME_CONSOLE_HPP */
