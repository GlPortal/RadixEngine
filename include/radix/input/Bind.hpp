#ifndef RADIX_BIND_HPP
#define RADIX_BIND_HPP

namespace radix {
	
struct Bind {
  int inputCode;
  int inputType;
  int action;
  float sensitivity;
  int deadZone;
	enum InputType : int8_t {
	  INPUT_TYPE_INVALID = -1,
	  KEYBOARD = 0,
	  MOUSE_BUTTON = 1,
	  MOUSE_AXIS = 2,
	  CONTROLLER_BUTTON = 3,
	  CONTROLLER_AXIS = 4,
	  INPUT_TYPE_MAX = 5
	};

  Bind()
    : inputCode(-1),
    inputType(-1),
    action(-1),
    sensitivity(0.0f),
    deadZone(0.0f) {}
  Bind(const int &inputCode, const int &inputType, const int &action, const float &sensitivity = 0.0f, const float &deadZone = 0.0f)
  : inputCode(inputCode),
  inputType(inputType),
  action(action),
  sensitivity(sensitivity),
  deadZone(deadZone) {}
};

}

#endif /* RADIX_BIND_HPP */