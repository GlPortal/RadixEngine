#include <radix/input/InputSource.hpp>

#include <SDL_keyboard.h>
#include <SDL_gamecontroller.h>

#include <algorithm>

#include <radix/core/event/EventDispatcher.hpp>

using namespace std;

namespace radix {

const InputSource::LookUpTable InputSource::mouseButtonLookUp = {
  {"mouse_button_left",   (int)MouseButton::Left},
  {"mouse_button_middle", (int)MouseButton::Middle},
  {"mouse_button_right",  (int)MouseButton::Right},
  {"mouse_button_aux_1",  (int)MouseButton::Aux1},
  {"mouse_button_aux_2",  (int)MouseButton::Aux2},
  {"mouse_button_aux_3",  (int)MouseButton::Aux3},
  {"mouse_button_aux_4",  (int)MouseButton::Aux4},
  {"mouse_button_aux_5",  (int)MouseButton::Aux5},
  {"mouse_button_aux_6",  (int)MouseButton::Aux6}
};

const InputSource::LookUpTable InputSource::controllerButtonLookUp = {
  {"button_a",        (int)SDL_CONTROLLER_BUTTON_A},
  {"button_b",        (int)SDL_CONTROLLER_BUTTON_B},
  {"button_x",        (int)SDL_CONTROLLER_BUTTON_X},
  {"button_y",        (int)SDL_CONTROLLER_BUTTON_Y},
  {"button_back",     (int)SDL_CONTROLLER_BUTTON_BACK},
  {"button_guide",    (int)SDL_CONTROLLER_BUTTON_GUIDE},
  {"button_start",    (int)SDL_CONTROLLER_BUTTON_START},
  {"left_stick",      (int)SDL_CONTROLLER_BUTTON_LEFTSTICK},
  {"right_stick",     (int)SDL_CONTROLLER_BUTTON_RIGHTSTICK},
  {"left_shoulder",   (int)SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
  {"right_shoulder",  (int)SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
  {"dpad_up",         (int)SDL_CONTROLLER_BUTTON_DPAD_UP},
  {"dpad_down",       (int)SDL_CONTROLLER_BUTTON_DPAD_DOWN},
  {"dpad_left",       (int)SDL_CONTROLLER_BUTTON_DPAD_LEFT},
  {"dpad_right",      (int)SDL_CONTROLLER_BUTTON_DPAD_RIGHT}
};

void InputSource::removeDispatcher(EventDispatcher &d) {
  dispatchers.erase(std::remove_if(dispatchers.begin(), dispatchers.end(), [&d](EventDispatcher &e) {
    return std::addressof(e) == std::addressof(d);
  }), dispatchers.end());
}

int InputSource::keyboardGetKeyFromString(const std::string &key) {
	return (int)SDL_GetScancodeFromName(key.c_str());
}

int InputSource::mouseGetButtonFromString(const std::string &buttonStr) {
  LookUpTable::const_iterator button = mouseButtonLookUp.find(buttonStr);
  if (button != mouseButtonLookUp.end()) {
    return button->second;
  } else {
    return -1;
  }
}

int InputSource::gameControllerGetButtonFromString(const std::string &buttonStr) {
  LookUpTable::const_iterator button = controllerButtonLookUp.find(buttonStr);
  if (button != controllerButtonLookUp.end()) {
    return button->second;
  } else {
    return -1;
  }
}

int InputSource::gameControllerGetAxisFromString(const std::string &axisStr) {
  if (axisStr == "stick_left") {
    return 0;
  } else if (axisStr == "stick_right") {
    return 1;
  } else {
    return -1;
  }
}

int InputSource::gameControllerGetTriggerFromString(const std::string &triggerStr) {
  if (triggerStr == "trigger_left") {
    return 0;
  } else if (triggerStr == "trigger_right") {
    return 1;
  } else {
    return -1;
  }
}

} /* namespace radix */
