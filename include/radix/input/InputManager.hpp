#ifndef RADIX_INPUT_MANAGER_HPP
#define RADIX_INPUT_MANAGER_HPP

#include <array>

#include <radix/core/math/Vector2f.hpp>
#include <radix/env/Config.hpp>
#include <radix/input/Channel.hpp>
#include <radix/input/ChannelListener.hpp>

namespace radix {

class BaseGame;
class EventDispatcher;

class InputManager : public ChannelListener {
public:
	enum Action : int8_t {
		ACTION_INVALID = -1,
		PLAYER_MOVE_ANALOGUE = 0,
		PLAYER_LOOK_ANALOGUE,
		PLAYER_JUMP,
		PLAYER_PORTAL_0,
		PLAYER_PORTAL_1,
		PLAYER_FORWARD,
		PLAYER_BACK,
		PLAYER_LEFT,
		PLAYER_RIGHT,
		GAME_PAUSE,
		GAME_QUIT,
		ACTION_MAX
	};

	InputManager() = delete;
	InputManager(BaseGame &baseGame);
	void setConfig(const Config &config);
	void init(EventDispatcher &event);

	virtual void channelChanged(const int &id) override;
	Vector2f getPlayerMovementVector() const;

	static bool isActionDigital(const int &act);

protected:
	BaseGame &game;
	Config config;
	std::map<int, Channel<float>> 	digitalChannels;
	std::map<int, Channel<Vector2f>> analogueChannels;

};

}

#endif /* RADIX_INPUT_MANAGER_HPP */
