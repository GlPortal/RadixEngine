#ifndef RADIX_INPUT_MANAGER_HPP
#define RADIX_INPUT_MANAGER_HPP

#include <unordered_map>

#include <radix/core/math/Vector2f.hpp>
#include <radix/env/Config.hpp>
#include <radix/input/Channel.hpp>
#include <radix/input/ChannelListener.hpp>

namespace radix {

class BaseGame;

class InputManager : public DigitalChannelListener, VectorChannelListener {
public:
  enum Action : int8_t {
    ACTION_INVALID = -1,
    PLAYER_MOVE_ANALOGUE = 0,
    PLAYER_LOOK_ANALOGUE,
    PLAYER_FORWARD,
    PLAYER_BACK,
    PLAYER_LEFT,
    PLAYER_RIGHT,
    PLAYER_JUMP,
    PLAYER_FIRE_PRIMARY,
    PLAYER_FIRE_SECONDARY,
    GAME_PAUSE,
    GAME_QUIT,
    GAME_START,
    ACTION_MAX
  };

  InputManager() = delete;
  InputManager(BaseGame *baseGame);

  void init();
  virtual void postInit() {}
  void reInit();
  virtual void postReInit() {}

  virtual void channelChanged(bool newValue, const int &id) override;     // DigitalChannelListener
  virtual void channelChanged(Vector2f newValue, const int &id) override; // VectorChannelListener

  Vector2f getPlayerMovementVector() const;

  static bool isActionDigital(const int &act);

protected:
  bool initialised;
  BaseGame *game;
  std::unordered_map<int, DigitalChannel> digitalChannels;
  std::unordered_map<int, VectorChannel>  vectorChannels;

};

}

#endif /* RADIX_INPUT_MANAGER_HPP */
