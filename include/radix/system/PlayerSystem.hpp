#ifndef RADIX_PLAYER_SYSTEM_HPP
#define RADIX_PLAYER_SYSTEM_HPP

#include <random>
#include <unordered_set>

#include <radix/system/System.hpp>
#include <radix/PlayerTask.hpp>

namespace radix {

class Entity;

static const float RUNNING_SPEED = 0.1f;
static const float JUMP_SPEED = 0.15f;
static const float HURT_VELOCITY = 0.18f;

static const std::array<const std::string, 2> PLAYER_PANTING_SOUND = {
  "/audio/sfx/character/fem_panting_1.ogg",
  "/audio/sfx/character/fem_panting_2.ogg"
};

static const std::array<const std::string, 2> PLAYER_JUMP_SOUND = {
  "/audio/sfx/character/fem_jump_1.ogg",
  "/audio/sfx/character/fem_jump_2.ogg"
};

static const std::array<const std::string, 2> PLAYER_FALL_SOUND = {
  "/audio/sfx/character/fem_fall_1.ogg",
  "/audio/sfx/character/fem_fall_2.ogg"
};

static const std::array<const std::string, 6> PLAYER_FOOT_SOUND = {
  "/audio/sfx/character/fem_foot_1.ogg",
  "/audio/sfx/character/fem_foot_2.ogg",
  "/audio/sfx/character/fem_foot_3.ogg",
  "/audio/sfx/character/fem_foot_4.ogg",
  "/audio/sfx/character/fem_foot_5.ogg",
  "/audio/sfx/character/fem_foot_6.ogg"
};

class PlayerSystem : public System {
protected:
  void mouseLook(Entity &entity);
  void runTasks(Entity &entity, TDelta dtime);
  bool allowedToRun(std::unordered_set<PlayerTask*> &blackList, PlayerTask *task);
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
  void update(TDelta dtime);
};

} /* namespace radix */

#endif /* RADIX_PLAYER_SYSTEM_HPP */
