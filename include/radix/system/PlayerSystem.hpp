#ifndef PLAYER_SYSTEM_HPP
#define PLAYER_SYSTEM_HPP

#include <random>

#include <radix/system/System.hpp>

namespace radix {

class Entity;

class PlayerSystem : public System {
protected:
  void mouseLook(Entity &entity);
  void move(Entity &entity, double dtime);

public:
  PlayerSystem(World&);
  ~PlayerSystem();

  const char* getName() const {
    return "PlayerSystem";
  }

  TypeId getTypeId() const {
    return System::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  bool runsBefore(const System&);
  void update(float dtime);
};

} /* namespace radix */

#endif /* PLAYER_SYSTEM_HPP */
