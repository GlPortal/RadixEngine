#include <radix/input/InputManager.hpp>

#include <cstdlib>

#include <radix/env/Config.hpp>
#include <radix/input/Bind.hpp>
#include <radix/core/diag/Throwables.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/entities/Player.hpp>
#include <radix/BaseGame.hpp>

namespace radix {

InputManager::InputManager(BaseGame &game) :
  game(game) {}

void InputManager::setConfig(const Config &config) {
  this->config = config;
}

void InputManager::init() {
  EventDispatcher& event = game.getWorld()->event;

  if (not config.getBindings()[PLAYER_MOVE_ANALOGUE].empty()) {
    analogueChannels[PLAYER_MOVE_ANALOGUE] = Channel<Vector2f>((ChannelListener*)this);
    analogueChannels[PLAYER_MOVE_ANALOGUE].init(PLAYER_MOVE_ANALOGUE, event, config.getBindings()[PLAYER_MOVE_ANALOGUE]);
  }

  analogueChannels[PLAYER_LOOK_ANALOGUE] = Channel<Vector2f>((ChannelListener*)this);
  analogueChannels[PLAYER_LOOK_ANALOGUE].init(PLAYER_LOOK_ANALOGUE, event, config.getBindings()[PLAYER_LOOK_ANALOGUE]);

  for (int id = 2; id < ACTION_MAX; ++id) {
    digitalChannels[id] = Channel<float>((ChannelListener*)this);
    digitalChannels[id].init(id, event, config.getBindings()[id]);
  }
}

void InputManager::reInit() {
  EventDispatcher& event = game.getWorld()->event;

  for (auto& el : analogueChannels) {
    el.second.reInit(event);
  }

  for (auto& el : digitalChannels) {
    el.second.reInit(event);
  }
}

void InputManager::close() {
  for (auto& el : analogueChannels) {
    el.second.close();
  }

  for (auto& el : digitalChannels) {
    el.second.close();
  }
}

void InputManager::channelChanged(const int &id) {
  entities::Player& player = game.getWorld()->getPlayer();

  switch(id) {
  case PLAYER_MOVE_ANALOGUE: {
    Vector2f movement = analogueChannels[PLAYER_MOVE_ANALOGUE].get();
    player.move(movement);
    break;
  }
  case PLAYER_LOOK_ANALOGUE: {
    Vector2f heading = analogueChannels[PLAYER_LOOK_ANALOGUE].get();
    player.changeHeading(heading);
    break;
  }
  case PLAYER_JUMP: {
    if (digitalChannels[PLAYER_JUMP].get()) {
      player.jump();
    }
    break;
  }
  case PLAYER_PORTAL_0: {
    //
  }
  case PLAYER_PORTAL_1: {
    //
  }
  case PLAYER_FORWARD:
  case PLAYER_BACK: {
    float moveY = digitalChannels[PLAYER_BACK].get() - digitalChannels[PLAYER_FORWARD].get();
    player.moveY(moveY);
    break;
  }
  case PLAYER_LEFT:
  case PLAYER_RIGHT: {
    float moveX = digitalChannels[PLAYER_RIGHT].get() - digitalChannels[PLAYER_LEFT].get();
    player.moveX(moveX);
    break;
  }
  case GAME_PAUSE: {

  }
  case GAME_QUIT: {
    if (digitalChannels[GAME_QUIT].get()) {
      game.close();
    }
    break;
  }
  default: {
    return;
  }
  }
}

Vector2f InputManager::getPlayerMovementVector() const {
  Vector2f movement = analogueChannels.at(PLAYER_MOVE_ANALOGUE).get();
  movement.x += digitalChannels.at(PLAYER_RIGHT).get() - digitalChannels.at(PLAYER_LEFT).get();
  movement.y += digitalChannels.at(PLAYER_BACK).get() - digitalChannels.at(PLAYER_FORWARD).get();
  return movement;
}

bool InputManager::isActionDigital(const int &act) {
  switch(act) {
  case PLAYER_JUMP:
  case PLAYER_PORTAL_0:
  case PLAYER_PORTAL_1:
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
