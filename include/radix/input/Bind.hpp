#ifndef RADIX_BIND_HPP
#define RADIX_BIND_HPP

namespace radix {
	
struct Bind {
  int8_t action;
  int8_t inputType;
  int8_t inputCode;
  float sensitivity;
  union {
    float deadZone, actPoint;
  };
	enum InputType : int8_t {
	  INPUT_TYPE_INVALID = -1,
	  KEYBOARD = 0,
	  MOUSE_BUTTON = 1,
	  MOUSE_AXIS = 2,
	  CONTROLLER_BUTTON = 3,
    CONTROLLER_AXIS = 4,
	  CONTROLLER_TRIGGER = 5,
	  INPUT_TYPE_MAX = 6
	};
  static bool isInputTypeDigital(const int& input) {
    switch (input) {
    case KEYBOARD:
    case MOUSE_BUTTON:
    case CONTROLLER_BUTTON: {
      return true;
      break;
    }
    case MOUSE_AXIS:
    case CONTROLLER_AXIS:
    case CONTROLLER_TRIGGER:
    case INPUT_TYPE_INVALID:
    case INPUT_TYPE_MAX: {
      return false;
      break;
    }
    }
  }

  Bind()
    :action(-1),
    inputType(-1),
    inputCode(-1),
    sensitivity(0.0f),
    deadZone(0.0f) {}
  Bind(const int &action, const int &inputType, const int &inputCode, const float &sensitivity = 0.0f, const float &deadZone = 0.5)
  : action(action),
  inputType(inputType),
  inputCode(inputCode),
  sensitivity(sensitivity),
  deadZone(deadZone) {}
};

}

#endif /* RADIX_BIND_HPP */