#ifndef RADIX_SIMULATION_PLAYER_HPP
#define RADIX_SIMULATION_PLAYER_HPP

#include <radix/simulation/Simulation.hpp>

namespace radix {
namespace simulation {

class Player final : public Simulation {
public:
  Player(World&, BaseGame *game);
  ~Player();

  const char* getName() const override;
  void update(TDelta dtime) override;
};

} /* namespace simulation */
} /* namespace radix */

#endif /* RADIX_SIMULATION_PLAYER_HPP */
