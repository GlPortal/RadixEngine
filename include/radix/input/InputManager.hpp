#ifndef RADIX_INPUT_MANAGER_HPP
#define RADIX_INPUT_MANAGER_HPP

#include <array>

#include <radix/env/Config.hpp>

namespace radix {
	
struct Vector3f;
class InputSource;

class InputManager {
public:
	enum Action : int8_t {
		ACTION_INVALID = -1,
		PLAYER_MOVE_X = 0,
		PLAYER_MOVE_Y = 1,
		PLAYER_LOOK_X = 2,
		PLAYER_LOOK_Y = 3,
		PLAYER_JUMP = 4,
		PLAYER_PORTAL_0 = 5,
		PLAYER_PORTAL_1 = 6,
		PLAYER_FORWARD = 7,
		PLAYER_BACK = 8,
		PLAYER_LEFT = 9,
		PLAYER_RIGHT = 10,
		GAME_PAUSE = 11,
		GAME_QUIT = 12,
		ACTION_MAX = 13
	};

	using StateArray = std::array<float, ACTION_MAX>;

	InputManager(InputSource &inputSource);

	void setConfig(radix::Config &config);
	void update();

	/**
   	* Gets the state of specified action.
   	* @throws std::out_of_range if action is out of bounds.
   	*/
	float getState(const int &state) const;
	Vector3f getPlayerMovementVector() const;

private:
	InputSource &inputSource;
	Config config;
	StateArray states;

};

}

#endif /* RADIX_INPUT_MANAGER_HPP */
