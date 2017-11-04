#include <radix/input/InputSource.hpp>

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_gamecontroller.h>

#include <algorithm>

#include <radix/core/event/EventDispatcher.hpp>

using namespace std;

namespace radix {

void InputSource::removeDispatcher(EventDispatcher &d) {
  dispatchers.erase(std::remove_if(dispatchers.begin(), dispatchers.end(), [&d](EventDispatcher &e) {
    return std::addressof(e) == std::addressof(d);
  }), dispatchers.end());
}

std::string InputSource::mouseButtonToString(const int &mouseButton) {
  switch ((MouseButton)mouseButton) {
    case MouseButton::MOUSE_BUTTON_LEFT:    return "mouse_left";
    case MouseButton::MOUSE_BUTTON_RIGHT:   return "mouse_right";
    case MouseButton::MOUSE_BUTTON_MIDDLE:  return "mouse_middle";
    default:                                return "";
  }
}

std::string InputSource::mouseAxisToString(const int &mouseAxis) {
  switch ((MouseAxis)mouseAxis) {
    case MouseAxis::MOUSE_AXIS_X: return "mouse_x";
    case MouseAxis::MOUSE_AXIS_Y: return "mouse_y";
    default:                      return "";
  }
}

int InputSource::keyboardGetKeyFromString(const std::string &key) {
	return (int)SDL_GetScancodeFromName(key.c_str());
}

int InputSource::mouseGetButtonFromString(const std::string &buttonStr) {
  if (buttonStr == "mouse_left") {
    return (int)MouseButton::MOUSE_BUTTON_LEFT;
  } else if (buttonStr == "mouse_middle") {
    return (int)MouseButton::MOUSE_BUTTON_MIDDLE;
  } else if (buttonStr == "mouse_right") {
    return (int)MouseButton::MOUSE_BUTTON_RIGHT;
  } else {
    return (int)MouseButton::MOUSE_BUTTON_INVALID;
  }
}

int InputSource::mouseGetAxisFromString(const std::string &axisStr) {
  if (axisStr == "mouse_x") {
    return (int)(MouseAxis::MOUSE_AXIS_X);
  } else if (axisStr == "mouse_y") {
    return (int)(MouseAxis::MOUSE_AXIS_Y);
  } else {
    return (int)(MouseAxis::MOUSE_AXIS_INVALID);
  }
}

int InputSource::gameControllerGetButtonFromString(const std::string &buttonStr) {
  if (buttonStr == "button_a") {
    return (int)SDL_CONTROLLER_BUTTON_A;
  } else if (buttonStr == "button_b") {
    return (int)SDL_CONTROLLER_BUTTON_B;
  } else if (buttonStr == "button_x") {
    return (int)SDL_CONTROLLER_BUTTON_X;
  } else if (buttonStr == "button_y") {
    return (int)SDL_CONTROLLER_BUTTON_Y;
  } else if (buttonStr == "button_back") {
    return (int)SDL_CONTROLLER_BUTTON_BACK;
  } else if (buttonStr == "button_guide") {
    return (int)SDL_CONTROLLER_BUTTON_GUIDE;
  } else if (buttonStr == "button_start") {
    return (int)SDL_CONTROLLER_BUTTON_START;
  } else if (buttonStr == "left_stick") {
    return (int)SDL_CONTROLLER_BUTTON_LEFTSTICK;
  } else if (buttonStr == "right_stick") {
    return (int)SDL_CONTROLLER_BUTTON_RIGHTSTICK;
  } else if (buttonStr == "left_shoulder") {
    return (int)SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
  } else if (buttonStr == "right_shoulder") {
    return (int)SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
  } else if (buttonStr == "dpad_up") {
    return (int)SDL_CONTROLLER_BUTTON_DPAD_UP;
  } else if (buttonStr == "dpad_down") {
    return (int)SDL_CONTROLLER_BUTTON_DPAD_DOWN;
  } else if (buttonStr == "dpad_left") {
    return (int)SDL_CONTROLLER_BUTTON_DPAD_LEFT;
  } else if (buttonStr == "dpad_right") {
    return (int)SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
  } else {
    return (int)SDL_CONTROLLER_BUTTON_INVALID;
  }
}

int InputSource::gameControllerGetAxisFromString(const std::string &axisStr) {
  if (axisStr == "left_stick_x") {
    return (int)SDL_CONTROLLER_AXIS_LEFTX;
  } else if (axisStr == "left_stick_y") {
    return (int)SDL_CONTROLLER_AXIS_LEFTY;
  } else if (axisStr == "right_stick_x") {
    return (int)SDL_CONTROLLER_AXIS_RIGHTX;
  } else if (axisStr == "right_stick_y") {
    return (int)SDL_CONTROLLER_AXIS_RIGHTY;
  } else if (axisStr == "left_trigger") {
    return (int)SDL_CONTROLLER_AXIS_TRIGGERLEFT;
  } else if (axisStr == "right_trigger") {
    return (int)SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
  } else {
    return (int)SDL_CONTROLLER_AXIS_INVALID;
  }
}

} /* namespace radix */
