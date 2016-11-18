#include <radix/system/PlayerSystem.hpp>

#include <iostream>

#include <radix/component/Player.hpp>
#include <radix/system/PhysicsSystem.hpp>

namespace radix {

PlayerSystem::PlayerSystem(World &w,BaseGame *game) :
  System(w), game(game) {
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
  std::unordered_set<PlayerTask *> blackList;

  auto it = player.tasks.begin();
  while (it != player.tasks.end()) {
    PlayerTask *task = it->second;

    if (allowedToRun(blackList, task)) {
      if (game) {
        task->task(game, dtime);
        blackList.insert(task->blackList.begin(), task->blackList.end());
      }
    }

    it++;
  }
}

bool PlayerSystem::allowedToRun(std::unordered_set<PlayerTask *> &blackList, PlayerTask *task) {
  if (blackList.empty()) {
    return true;
  }

  auto found = blackList.find(task);
  if (found == blackList.end()) {
    return true;
  } else {
    return false;
  }
}

bool PlayerSystem::runsBefore(const System &sys) {
  return sys.getTypeId() == System::getTypeId<PhysicsSystem>();
}

void PlayerSystem::update(TDelta dtime) {
  mouseLook(world.getPlayer());
  runTasks(world.getPlayer(), dtime);
}

} /* namespace radix */
