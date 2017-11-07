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
    KeyPressedEvent(InputSource &source, const KeyboardKey &key, const KeyboardModifier &mod)
      : source(source), key(key), mod(mod) {}
  };
  struct KeyReleasedEvent : public Event {
    radix_event_declare("radix/InputSource:KeyReleased")
    InputSource &source;
    const KeyboardKey key;
    const KeyboardModifier mod;
    KeyReleasedEvent(InputSource &source, const KeyboardKey &key, const KeyboardModifier &mod)
      : source(source), key(key), mod(mod) {}
  };

  /* ================= */
  /*   Mouse buttons   */
  /* ================= */
  enum class MouseButton : int8_t {
    MOUSE_BUTTON_INVALID = -1,
    MOUSE_BUTTON_LEFT = 0,
    MOUSE_BUTTON_MIDDLE = 1,
    MOUSE_BUTTON_RIGHT = 2,
    MOUSE_BUTTON_AUX1,
    MOUSE_BUTTON_AUX2,
    MOUSE_BUTTON_AUX3,
    MOUSE_BUTTON_AUX4,
    MOUSE_BUTTON_AUX5,
    MOUSE_BUTTON_AUX6,
    MOUSE_BUTTON_MAX
  };
  enum class MouseAxis : int8_t {
    MOUSE_AXIS_INVALID = -1,
    MOUSE_AXIS_X = 0,
    MOUSE_AXIS_Y = 1,
    MOUSE_AXIS_MAX = 2
  };
  struct MouseButtonPressedEvent : public Event {
    radix_event_declare("radix/InputSource:MouseButtonPressed")
    InputSource &source;
    const MouseButton button;
    MouseButtonPressedEvent(InputSource &source, const MouseButton &button)
      : source(source), button(button) {}
  };
  struct MouseButtonReleasedEvent : public Event {
    radix_event_declare("radix/InputSource:MouseButtonReleased")
    InputSource &source;
    const MouseButton button;
    MouseButtonReleasedEvent(InputSource &source, const MouseButton &button)
      : source(source), button(button) {}
  };
  struct MouseMovedEvent : public Event {
    radix_event_declare("radix/InputSource:MouseMoved")
    InputSource &source;
    const int xrel, yrel;
    MouseMovedEvent(InputSource &source, const int& xrel, const int& yrel)
      : source(source), xrel(xrel), yrel(yrel) {}
  };
  struct MouseWheelScrolledEvent : public Event {
    radix_event_declare("radix/InputSource:MouseWheelScrolled")
    InputSource &source;
    const int x, y;
    MouseWheelScrolledEvent(InputSource &source, const int &x, const int &y)
      : source(source), x(x), y(y) {}
  };

public:
  /* ================= */
  /*    Controller     */
  /* ================= */
  using ControllerButton = int;
  using ControllerAxis = int;
  using ControllerIndex = int;

  struct ControllerButtonPressedEvent : public Event {
    radix_event_declare("radix/InputSource:ControllerButtonPressed")
    InputSource &source;
    const ControllerButton button;
    const ControllerIndex index;
    ControllerButtonPressedEvent(InputSource &source, const ControllerButton &button, const ControllerIndex &index)
      : source(source), button(button), index(index) {}
  };
  struct ControllerButtonReleasedEvent : public Event {
    radix_event_declare("radix/InputSource:ControllerButtonReleased")
    InputSource &source;
    const ControllerButton button;
    const ControllerIndex index;
    ControllerButtonReleasedEvent(InputSource &source, const ControllerButton &button, const ControllerIndex &index)
      : source(source), button(button), index(index) {}
  };
  struct ControllerAddedEvent : public Event {
    radix_event_declare("radix/InputSource:ControllerAddedEvent")
    InputSource &source;
    const ControllerIndex index;
    ControllerAddedEvent(InputSource &source, const ControllerIndex &index)
      : source(source), index(index) {}
  };
  struct ControllerRemovedEvent : public Event {
    radix_event_declare("radix/InputSource:ControllerRemoved")
    InputSource &source;
    const ControllerIndex index;
    ControllerRemovedEvent(InputSource &source, const ControllerIndex &index)
      : source(source), index(index) {}
  };

  /* =============== */
  /*      Window     */
  /* =============== */
  struct WindowShownEvent : public Event {
    radix_event_declare("radix/InputSource:WindowShown")
    InputSource &source;
    const Uint32 windowID;
    WindowShownEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowHiddenEvent : public Event {
    radix_event_declare("radix/InputSource:WindowHidden")
    InputSource &source;
    const Uint32 windowID;
    WindowHiddenEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowExposedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowExposed")
    InputSource &source;
    const Uint32 windowID;
    WindowExposedEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowMovedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowMoved")
    InputSource &source;
    const Uint32 windowID;
    const Sint32 to;
    const Sint32 from;
    WindowMovedEvent(InputSource &source, const Uint32 &windowID, const Sint32 &to, const Sint32 &from)
        : source(source), windowID(windowID), to(to), from(from) {}
  };
  struct WindowResizedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowResized")
    InputSource &source;
    const Uint32 windowID;
    const Sint32 x;
    const Sint32 y;
    WindowResizedEvent(InputSource &source, const Uint32 & windowID, const Sint32 &x, const Sint32 &y)
        : source(source), windowID(windowID), x(x), y(y) {}
  };
  struct WindowSizeChangedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowSizeChanged")
    InputSource &source;
    const Uint32 windowID;
    WindowSizeChangedEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };

  struct WindowMinimizedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowMinimized")
    InputSource &source;
    const Uint32 windowID;
    WindowMinimizedEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowMaximizedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowMaximized")
    InputSource &source;
    const Uint32 windowID;
    WindowMaximizedEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowRestoredEvent : public Event {
    radix_event_declare("radix/InputSource:WindowRestored")
    InputSource &source;
    const Uint32 windowID;
    WindowRestoredEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowEnterEvent : public Event {
    radix_event_declare("radix/InputSource:EnterExposed")
    InputSource &source;
    const Uint32 windowID;
    WindowEnterEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowLeaveEvent : public Event {
    radix_event_declare("radix/InputSource:WindowLeave")
    InputSource &source;
    const Uint32 windowID;
    WindowLeaveEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowFocusGainedEvent : public Event {
    radix_event_declare("radix/InputSource:WindowFocusGained")
    InputSource &source;
    const Uint32 windowID;
    WindowFocusGainedEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowFocusLostEvent : public Event {
    radix_event_declare("radix/InputSource:FocusLost")
    InputSource &source;
    const Uint32 windowID;
    WindowFocusLostEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };
  struct WindowCloseEvent : public Event {
    radix_event_declare("radix/InputSource:WindowClose")
    InputSource &source;
    const Uint32 windowID;
    WindowCloseEvent(InputSource &source, const Uint32 &windowID)
        : source(source), windowID(windowID) {}
  };

  void addDispatcher(EventDispatcher &d) {
    dispatchers.push_back(d);
  }
  void removeDispatcher(EventDispatcher &d);

  virtual void processEvents() = 0;
  virtual void keyPressed(const KeyboardKey &key, const KeyboardModifier &mod) = 0;
  virtual void keyReleased(const KeyboardKey &key, const KeyboardModifier &mod) = 0;
  virtual bool isKeyDown(const KeyboardKey &key) = 0;
  virtual void controllerButtonPressed(const ControllerButton &button, const ControllerIndex &index) = 0;
  virtual void controllerButtonReleased(const ControllerButton &button, const ControllerIndex &index) = 0;
  virtual bool isControllerButtonDown(const ControllerButton &button, const ControllerIndex &index) = 0;
  virtual int getControllerAxisValue(const ControllerAxis &axis, const ControllerIndex &index) = 0;
  virtual bool isMouseButtonDown(const int &button) = 0;
  virtual int getRelativeMouseAxisValue(const int &axis) = 0;
  virtual void getRelativeMouseState(int *dx, int *dy) = 0;
  virtual std::string getCharBuffer() = 0;
  virtual void addToBuffer(const std::string &character) = 0;
  virtual void clearBuffer() = 0;
  virtual void truncateCharBuffer() = 0;
  virtual void clear() = 0;

  static std::string mouseButtonToString(const int &mouseButton);
  static std::string mouseAxisToString(const int &mouseAxis);
  static int keyboardGetKeyFromString(const std::string &key);
  static int mouseGetButtonFromString(const std::string &button);
  static int mouseGetAxisFromString(const std::string &axis);
  static int gameControllerGetButtonFromString(const std::string &button);
  static int gameControllerGetAxisFromString(const std::string &axis);
};

} /* namespace radix */

#endif /* RADIX_INPUT_SOURCE_HPP */