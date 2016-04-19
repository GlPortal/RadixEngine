#ifndef RADIX_INPUT_SOURCE_HPP
#define RADIX_INPUT_SOURCE_HPP

#include <functional>
#include <string>
#include <vector>

#include <radix/core/event/Event.hpp>

namespace radix {

class EventDispatcher;

class InputSource {
protected:
  std::vector<std::reference_wrapper<EventDispatcher>> dispatchers;

public:
  /* ============ */
  /*   Keyboard   */
  /* ============ */
  using KeyboardKey = int;
  using KeyboardModifier = int;

  struct KeyPressedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/InputSource:KeyPressed";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    InputSource &source;
    const KeyboardKey key;
    const KeyboardModifier mod;
    KeyPressedEvent(InputSource &source, KeyboardKey key, KeyboardModifier mod)
      : source(source), key(key), mod(mod) {}
  };
  struct KeyReleasedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/InputSource:KeyReleased";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    InputSource &source;
    const KeyboardKey key;
    const KeyboardModifier mod;
    KeyReleasedEvent(InputSource &source, KeyboardKey key, KeyboardModifier mod)
      : source(source), key(key), mod(mod) {}
  };

  /* ================= */
  /*   Mouse buttons   */
  /* ================= */
  enum class MouseButton : uint8_t {
    Left = 0,
    Right = 1,
    Middle = 2,
    Aux1,
    Aux2,
    Aux3,
    Aux4,
    Aux5,
    Aux6,
    Aux7,
    Aux8,
    Unknown = 0xFF
  };
  struct MouseButtonPressedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/InputSource:MouseButtonPressed";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    InputSource &source;
    const MouseButton button;
    MouseButtonPressedEvent(InputSource &source, MouseButton button)
      : source(source), button(button) {}
  };
  struct MouseButtonReleasedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/InputSource:MouseButtonReleased";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    InputSource &source;
    const MouseButton button;
    MouseButtonReleasedEvent(InputSource &source, MouseButton button)
      : source(source), button(button) {}
  };

  /* =============== */
  /*   Mouse wheel   */
  /* =============== */
  struct MouseWheelScrolledEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/InputSource:MouseWheelScrolled";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    InputSource &source;
    const int x, y;
    MouseWheelScrolledEvent(InputSource &source, int x, int y)
      : source(source), x(x), y(y) {}
  };


  void addDispatcher(EventDispatcher &d) {
    dispatchers.push_back(d);
  }
  void removeDispatcher(EventDispatcher &d);

  virtual void processEvents() = 0;
  virtual void keyPressed(KeyboardKey key, KeyboardModifier mod) = 0;
  virtual void keyReleased(KeyboardKey key, KeyboardModifier mod) = 0;
  virtual bool isKeyDown(KeyboardKey key) = 0;
  virtual std::string getCharBuffer() = 0;
  virtual void addToBuffer(const std::string &character) = 0;
  virtual void clearBuffer() = 0;
  virtual void truncateCharBuffer() = 0;
  virtual void clear() = 0;
};

} /* namespace radix */

#endif /* RADIX_INPUT_SOURCE_HPP */
