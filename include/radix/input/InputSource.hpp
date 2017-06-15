#ifndef RADIX_INPUT_SOURCE_HPP
#define RADIX_INPUT_SOURCE_HPP

#include <functional>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

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
    radix_event_declare("radix/InputSource:KeyPressed")
    InputSource &source;
    const KeyboardKey key;
    const KeyboardModifier mod;
    KeyPressedEvent(InputSource &source, KeyboardKey key, KeyboardModifier mod)
      : source(source), key(key), mod(mod) {}
  };
  struct KeyReleasedEvent : public Event {
    radix_event_declare("radix/InputSource:KeyReleased")
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
    radix_event_declare("radix/InputSource:MouseButtonPressed")
    InputSource &source;
    const MouseButton button;
    MouseButtonPressedEvent(InputSource &source, MouseButton button)
      : source(source), button(button) {}
  };
  struct MouseButtonReleasedEvent : public Event {
    radix_event_declare("radix/InputSource:MouseButtonReleased")
    InputSource &source;
    const MouseButton button;
    MouseButtonReleasedEvent(InputSource &source, MouseButton button)
      : source(source), button(button) {}
  };

  /* =============== */
  /*   Mouse wheel   */
  /* =============== */
  struct MouseWheelScrolledEvent : public Event {
    radix_event_declare("radix/InputSource:MouseWheelScrolled")
    InputSource &source;
    const int x, y;
    MouseWheelScrolledEvent(InputSource &source, int x, int y)
      : source(source), x(x), y(y) {}
  };

  /* =============== */
  /*      Window     */
  /* =============== */
  struct WindowShownEvent : public Event {
    radix_event_declare("radix/InputSource:WindowShown")
    InputSource &source;
    Uint32 windowID;
    WindowShownEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowHiddenEvent : public Event {
    radix_event_declare("radix/InputSource:WindowHidden")
    InputSource &source;
    Uint32 windowID;
    WindowHiddenEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowExposedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowExposed")
    InputSource &source;
    Uint32 windowID;
    WindowExposedEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowMovedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowMoved")
    InputSource &source;
    Uint32 windowID;
    Sint32 to;
    Sint32 from;
    WindowMovedEvent(InputSource &source, Uint32 windowID, Sint32 to, Sint32 from)
        : source(source), windowID(windowID), to(to), from(from) {}
  };
  struct WindowResizedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowResized")
    InputSource &source;
    Uint32 windowID;
    Sint32 x;
    Sint32 y;
    WindowResizedEvent(InputSource &source, Uint32 windowID, Sint32 x, Sint32 y)
        : source(source), windowID(windowID), x(x), y(y) {}
  };
  struct WindowSizeChangedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowSizeChanged")
    InputSource &source;
    Uint32 windowID;
    WindowSizeChangedEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };

  struct WindowMinimizedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowMinimized")
    InputSource &source;
    Uint32 windowID;
    WindowMinimizedEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowMaximizedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowMaximized")
    InputSource &source;
    Uint32 windowID;
    WindowMaximizedEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowRestoredEvent : public Event {
    radix_event_declare("radix/InputSource:WindowRestored")
    InputSource &source;
    Uint32 windowID;
    WindowRestoredEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowEnterEvent : public Event {
    radix_event_declare("radix/InputSource:EnterExposed")
    InputSource &source;
    Uint32 windowID;
    WindowEnterEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowLeaveEvent : public Event {
    radix_event_declare("radix/InputSource:WindowLeave")
    InputSource &source;
    Uint32 windowID;
    WindowLeaveEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowFocusGainedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowFocusGained")
    InputSource &source;
    Uint32 windowID;
    WindowFocusGainedEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowFocusLostEvent : public Event {
    radix_event_declare("radix/InputSource:FocusLost")
    InputSource &source;
    Uint32 windowID;
    WindowFocusLostEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowCloseEvent : public Event {
    radix_event_declare("radix/InputSource:WindowClose")
    InputSource &source;
    Uint32 windowID;
    WindowCloseEvent(InputSource &source, Uint32 windowID)
        : source(source), windowID(windowID) {}
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
