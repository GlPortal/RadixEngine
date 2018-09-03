#include <radix/input/InputManager.hpp>

#include <cstdlib>

#include <radix/env/Config.hpp>
#include <radix/input/Bind.hpp>
#include <radix/core/diag/Throwables.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/entities/Player.hpp>
#include <radix/BaseGame.hpp>

namespace radix {

InputManager::InputManager(BaseGame *game) :
  initialised(false),
  game(game) {}

void InputManager::init() {
  if (!initialised) {
    if (!game->getConfig().isLoaded()) {
      throw Exception::Error("InputManager", "Tried to initialize InputManager without config.");
    }

    initialised = true;

    EventDispatcher& event = game->getWorld()->event;
    const auto &bindings = game->getConfig().getBindings();

    vectorChannels.insert({PLAYER_MOVE_ANALOGUE, VectorChannel(this)});
    vectorChannels[PLAYER_MOVE_ANALOGUE].init(PLAYER_MOVE_ANALOGUE, event, bindings[PLAYER_MOVE_ANALOGUE]);

    vectorChannels.insert({PLAYER_LOOK_ANALOGUE, VectorChannel(this)});
    vectorChannels[PLAYER_LOOK_ANALOGUE].init(PLAYER_LOOK_ANALOGUE, event, bindings[PLAYER_LOOK_ANALOGUE]);

    for (int i = PLAYER_FORWARD; i < ACTION_MAX; ++i) {
      digitalChannels.insert({i, DigitalChannel(this)});
      digitalChannels[i].init(i, event, bindings[i]);
    }

    postInit();
  } else {
    reInit();
    postReInit();
  }
}

void InputManager::reInit() {
  EventDispatcher& event = game->getWorld()->event;

  for (auto& el : vectorChannels) {
    el.second.reInit(event);
  }

  for (auto& el : digitalChannels) {
    el.second.reInit(event);
  }
}

void InputManager::channelChanged(bool newValue, const int &id) {
  entities::Player& player = game->getWorld()->getPlayer();

  switch(id) {
  case PLAYER_JUMP: {
    if (newValue) {
      player.jump();
    }
    break;
  }
  case PLAYER_FORWARD:
  case PLAYER_BACK: {
    player.moveY((float)digitalChannels[PLAYER_BACK].get()-(float)digitalChannels[PLAYER_FORWARD].get());
    break;
  }
  case PLAYER_LEFT:
  case PLAYER_RIGHT: {
    player.moveX((float)digitalChannels[PLAYER_RIGHT].get()-(float)digitalChannels[PLAYER_LEFT].get());
    break;
  }
  case GAME_QUIT: {
    if (newValue) {
      game->close();
    }
    break;
  }
  default: {
    break;
  }
  }
}

void InputManager::channelChanged(Vector2f newValue, const int &id) {
  entities::Player& player = game->getWorld()->getPlayer();

  switch(id) {
  case PLAYER_MOVE_ANALOGUE: {
    player.move(newValue);
    break;
  }
  case PLAYER_LOOK_ANALOGUE: {
    player.changeHeading(newValue);
    break;
  }
  default: {
    break;
  }
  }
}

Vector2f InputManager::getPlayerMovementVector() const {
  return vectorChannels.at(PLAYER_MOVE_ANALOGUE).get();
}

bool InputManager::isActionDigital(const int &act) {
  switch(act) {
  case PLAYER_JUMP:
  case PLAYER_FIRE_PRIMARY:
  case PLAYER_FIRE_SECONDARY:
  case PLAYER_FORWARD:
  case PLAYER_BACK:
  case PLAYER_LEFT:
  case PLAYER_RIGHT :
  case GAME_PAUSE :
  case GAME_QUIT : {
    return true;
    break;
  }
  case PLAYER_MOVE_ANALOGUE:
  case PLAYER_LOOK_ANALOGUE:
  case ACTION_MAX :
  case ACTION_INVALID :
  default : {
    return false;
    break;
  }
  }
}

}
