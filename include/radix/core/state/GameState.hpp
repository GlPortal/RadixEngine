#ifndef RADIX_GAMESTATE_HPP
#define RADIX_GAMESTATE_HPP

#include <radix/BaseGame.hpp>
#include <radix/core/event/Event.hpp>

namespace radix {

class GameState {
public:
  struct WinEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/GameState:Win";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    WinEvent() {}
  };

public:
  void handleInput(BaseGame &game);
};

} /* namespace radix */

#endif /* RADIX_GAMESTATE_HPP */
