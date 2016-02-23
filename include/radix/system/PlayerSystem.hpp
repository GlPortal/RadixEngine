#ifndef PLAYER_SYSTEM_HPP
#define PLAYER_SYSTEM_HPP

#include <random>

#include <radix/scene/Scene.hpp>

namespace radix {

class PlayerSystem {
private:
  Scene *scene;
  std::mt19937 generator;

public:
  PlayerSystem();
  ~PlayerSystem();
  void setScene(Scene *scene);
  void update(float dtime);

  void mouseLook(Entity&);
  void move(Entity&, double dtime);
};

} /* namespace radix */

#endif /* PLAYER_SYSTEM_HPP */
