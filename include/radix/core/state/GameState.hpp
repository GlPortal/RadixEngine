#ifndef RADIX_GAMESTATE_HPP
#define RADIX_GAMESTATE_HPP

#include <radix/BaseGame.hpp>
#include <radix/core/event/Event.hpp>

namespace radix {

class GameState {
public:
  struct WinEvent : public Event {
    radix_event_declare("radix/GameState:Win")

    WinEvent() {}
  };

public:
  void handleInput(BaseGame &game);
};

} /* namespace radix */

#endif /* RADIX_GAMESTATE_HPP */
