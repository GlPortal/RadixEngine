#include <radix/simulation/Player.hpp>

#include <iostream>

// #include <radix/component/Player.hpp>
// #include <radix/system/PhysicsSystem.hpp>

namespace radix {
namespace simulation {

Player::Player(World &world, BaseGame*) :
  Simulation(world) {
}

Player::~Player() {
}

const char* Player::getName() const {
  return "Player";
}

void Player::update(TDelta dtime) {
  (void) dtime;
}

} /* namespace simulation */
} /* namespace radix */
