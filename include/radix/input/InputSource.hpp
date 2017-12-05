#ifndef RADIX_INPUT_SOURCE_HPP
#define RADIX_INPUT_SOURCE_HPP

#include <functional>
#include <string>
#include <vector>
#include <map>

#include <SDL2/SDL.h>

#include <radix/core/event/Event.hpp>
#include <radix/core/math/Vector2f.hpp>

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
  using MouseAxisValue = Vector2f;

  enum class MouseButton : int8_t {
    Invalid = -1,
    Left = 0,
    Middle = 1,
    Right = 2,
    Aux1,
    Aux2,
    Aux3,
    Aux4,
    Aux5,
    Aux6,
    Max
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
  struct MouseAxisEvent : public Event {
    radix_event_declare("radix/InputSource:MouseMoved")
    InputSource &source;
    const MouseAxisValue value;
    MouseAxisEvent(InputSource &source, const MouseAxisValue &value)
      : source(source), value(value) {}
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
  using ControllerAxis = int;     // 0/1 for left/right respectively
  using ControllerTrigger = int;  // 0/1 for left/right respectively
  using ControllerIndex = int;
  using ControllerAxisValue = Vector2f;
  using ControllerTriggerValue = float;

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
  struct ControllerAxisEvent : public Event {
    radix_event_declare("radix/InputSource:ControllerAxis")
    InputSource &source;
    const ControllerAxis axis;
    const ControllerAxisValue value;
    const ControllerIndex index;
    ControllerAxisEvent(InputSource &source, const ControllerAxis &axis, const ControllerAxisValue &value, const ControllerIndex &index)
      : source(source), axis(axis), value(value), index(index) {}
  };
  struct ControllerTriggerEvent : public Event {
    radix_event_declare("radix/InputSource:ControllerTrigger")
    InputSource &source;
    const ControllerTrigger trigger;
    const ControllerTriggerValue value;
    const ControllerIndex index;
    ControllerTriggerEvent(InputSource &source, const ControllerTrigger &trigger, const ControllerTriggerValue &value , const ControllerIndex &index)
      : source(source), trigger(trigger), value(value), index(index) {}
  };
  struct ControllerAddedEvent : public Event {
    radix_event_declare("radix/InputSource:ControllerAdded")
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
    const Sint32 x;
    const Sint32 y;
    WindowMovedEvent(InputSource &source, Uint32 windowID, Sint32 x, Sint32 y)
        : source(source), windowID(windowID), x(x), y(y) {}
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
  virtual float getControllerAxisValue(const ControllerAxis &axis, const ControllerIndex &index) = 0;
  virtual bool isMouseButtonDown(const int &button) = 0;
  virtual float getRelativeMouseAxisValue(const int &axis) = 0;
  virtual void getRelativeMouseState(int *dx, int *dy) = 0;
  virtual std::string getCharBuffer() = 0;
  virtual void addToBuffer(const std::string &character) = 0;
  virtual void clearBuffer() = 0;
  virtual void truncateCharBuffer() = 0;
  virtual void clear() = 0;

  using LookUpTable = std::map<std::string, int>;

private:
  static const LookUpTable mouseButtonLookUp;
  static const LookUpTable controllerButtonLookUp;

public:
  static int keyboardGetKeyFromString(const std::string &keyStr);
  static int mouseGetButtonFromString(const std::string &buttonStr);
  static int gameControllerGetButtonFromString(const std::string &buttonStr);
  static int gameControllerGetAxisFromString(const std::string &axisStr);
  static int gameControllerGetTriggerFromString(const std::string &triggerStr);
};

} /* namespace radix */

#endif /* RADIX_INPUT_SOURCE_HPP */
