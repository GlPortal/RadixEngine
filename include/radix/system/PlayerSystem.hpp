#ifndef PLAYER_SYSTEM_HPP
#define PLAYER_SYSTEM_HPP

#include <random>

#include <radix/system/System.hpp>

namespace radix {

class PlayerSystem : public System {
public:
  PlayerSystem(World&);
  ~PlayerSystem();

  const char* getName() const {
    return "PlayerSystem";
  }
  bool runsBefore(const System&);
  void update(float dtime);
};

} /* namespace radix */

#endif /* PLAYER_SYSTEM_HPP */
