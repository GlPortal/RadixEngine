#include <radix/system/PlayerSystem.hpp>

#include <iostream>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keyboard.h>
#include <radix/core/math/Math.hpp>
#include <radix/env/Environment.hpp>
#include <radix/component/Health.hpp>
#include <radix/component/SoundSource.hpp>
#include <radix/component/RigidBody.hpp>
#include <radix/component/Player.hpp>
#include <radix/system/PhysicsSystem.hpp>

namespace radix {

PlayerSystem::PlayerSystem(World &w) :
  System(w) {
}

PlayerSystem::~PlayerSystem() {
}

void PlayerSystem::mouseLook(Entity &entity) {
  Player &player = entity.getComponent<Player>();
  if (player.frozen) {
    return;
  }
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);

  // Apply mouse movement to view
  //Vector3f &rotation = entity.getComponent<Transform>().rotation;
  if (world.getConfig().isLoaded()) {
    player.headAngle.attitude -= rad(mousedy * world.getConfig().getSensitivity());
    player.headAngle.heading  -= rad(mousedx * world.getConfig().getSensitivity());
  } else {
    player.headAngle.attitude -= rad(mousedy * 0.30);
    player.headAngle.heading  -= rad(mousedx * 0.30);
  }
  player.headAngle.tilt *= 0.8;

  // Restrict rotation in horizontal axis
  player.headAngle.attitude = Math::clamp(player.headAngle.attitude, rad(-89.99), rad(89.99));
}

void PlayerSystem::runTasks(Entity &entity, TDelta dtime) {
  Player &player = entity.getComponent<Player>();
  std::list<PlayerTask *> blackList;

  auto it = player.tasks.begin();
  while (it != player.tasks.end()) {
    PlayerTask *task = it->second;

    if (allowedToRun(blackList, task)) {
      task->task(world, dtime);
      blackList.insert(blackList.end(), task->blackList.begin(), task->blackList.end());
    }

    it++;
  }
}

bool PlayerSystem::allowedToRun(std::list<PlayerTask *> &blackList, PlayerTask *task) {
  if (blackList.empty()) {
    return true;
  }

  for (PlayerTask *blackTask : blackList) {
    if (task->getName() != blackTask->getName()) {
      return true;
    }
  }
  return false;
}

bool PlayerSystem::runsBefore(const System &sys) {
  return sys.getTypeId() == System::getTypeId<PhysicsSystem>();
}

void PlayerSystem::update(TDelta dtime) {
  mouseLook(world.getPlayer());
  runTasks(world.getPlayer(), dtime);
}

} /* namespace radix */
