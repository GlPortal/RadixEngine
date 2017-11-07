#include <radix/input/InputManager.hpp>

#include <cstdlib>

#include <radix/env/Config.hpp>
#include <radix/input/InputSource.hpp>
#include <radix/input/Bind.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/core/diag/Throwables.hpp>

namespace radix {

InputManager::InputManager(InputSource &inputSource) :
	inputSource(inputSource) {
		states.fill(0.0f);
	}

void InputManager::setConfig(radix::Config &config) {
	this->config = config;
}

void InputManager::update() {
	states.fill(0.0f);

	for (const Bind &bind : config.getBindings()) {
		switch (bind.inputType) {
		case Bind::KEYBOARD: {
			states[bind.action] += float(inputSource.isKeyDown(bind.inputCode));
		break;
		}
		case Bind::CONTROLLER_BUTTON: {
			states[bind.action] += float(inputSource.isControllerButtonDown(bind.inputCode, 0));
			break;
		}
		case Bind::CONTROLLER_AXIS: {
			int value = inputSource.getControllerAxisValue(bind.inputCode, 0);
			if (std::abs(value) > bind.deadZone) {
				states[bind.action] += float(value) * bind.sensitivity / 32767.0f;
			}
			break;
		}
		case Bind::MOUSE_AXIS: {
			states[bind.action] += float(inputSource.getRelativeMouseAxisValue(bind.inputCode))*bind.sensitivity;
			break;
		}
		case Bind::MOUSE_BUTTON: {
			states[bind.action] += float(inputSource.isMouseButtonDown(bind.inputCode));
			break;
		}
		case Bind::INPUT_TYPE_INVALID:
		case Bind::INPUT_TYPE_MAX:
		default : break;
    }
  }

  states[PLAYER_MOVE_X] += states[PLAYER_RIGHT] - states[PLAYER_LEFT];
  states[PLAYER_MOVE_Y] += states[PLAYER_BACK] - states[PLAYER_FORWARD];

  for (int i(0); i < ACTION_MAX; ++i) {
  	if (i != PLAYER_LOOK_X and i != PLAYER_LOOK_Y) {
  		states[i] = Math::clamp(states[i], -1.0f, 1.0f);
  	}
  }
}

float InputManager::getState(const int &state) const {
	if (state >= 0 and state < ACTION_MAX) {
		return states[state];
	} else {
		throw Exception::Error("InputManager", "Requested state out of bounds");
	}
}

Vector3f InputManager::getPlayerMovementVector() const {
	return Vector3f(states[PLAYER_MOVE_X], 0.0f, states[PLAYER_MOVE_Y]);
}

}
