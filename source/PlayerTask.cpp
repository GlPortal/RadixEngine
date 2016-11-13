#include <radix/PlayerTask.hpp>
#include <radix/env/Environment.hpp>
#include <radix/component/SoundSource.hpp>
#include <radix/component/Player.hpp>
#include <radix/system/PlayerSystem.hpp>

namespace radix {

PlayerTestTask::PlayerTestTask() {
  task = [] (World &world, TDelta dtime) {
    //Util::Log() << "Test";
  };
}

std::string PlayerTestTask::getName() {
  return "PlayerTestTask";
}

PlayerMoveTask::PlayerMoveTask() {
  task = [] (World &world, TDelta dtime) {
    (void) dtime;
    Player &player = world.getPlayer().getComponent<Player>();
    if (player.frozen) {
      return;
    }
    InputSource &input = world.input;
    bool movingFwd     = input.isKeyDown(SDL_SCANCODE_W) or input.isKeyDown(SDL_SCANCODE_UP),
      movingBack    = input.isKeyDown(SDL_SCANCODE_S) or input.isKeyDown(SDL_SCANCODE_DOWN),
      strafingLeft  = input.isKeyDown(SDL_SCANCODE_A) or input.isKeyDown(SDL_SCANCODE_LEFT),
      strafingRight = input.isKeyDown(SDL_SCANCODE_D) or input.isKeyDown(SDL_SCANCODE_RIGHT),
      jumping       = input.isKeyDown(SDL_SCANCODE_SPACE) or
                      input.isKeyDown(SDL_SCANCODE_BACKSPACE);
    float rot = player.headAngle.heading;
    Vector3f movement;
    KinematicCharacterController &controller = *world.getPlayer().getComponent<Player>().controller;
    Transform &plrTform = world.getPlayer().getComponent<Transform>();
    plrTform.privSetPosition(world.getPlayer().getComponent<Player>().obj->getWorldTransform().getOrigin());

    if (jumping and controller.canJump()) {
      std::uniform_int_distribution<> dis(0, PLAYER_JUMP_SOUND.size()-1);
      world.getPlayer().getComponent<SoundSource>().playSound(
        Environment::getDataDir() + PLAYER_JUMP_SOUND[dis(Util::Rand)]);
      controller.jump();
    }
    if (movingFwd) {
      movement.x += -sin(rot);
      movement.z += -cos(rot);
    }
    if (movingBack) {
      movement.x += sin(rot);
      movement.z += cos(rot);
    }
    if (strafingLeft) {
      movement.x += -cos(rot);
      movement.z += sin(rot);
    }
    if (strafingRight) {
      movement.x += cos(rot);
      movement.z += -sin(rot);
    }

    movement *= RUNNING_SPEED;
    controller.setWalkDirection(movement);

    if (controller.onGround()) {
      player.stepCounter += std::sqrt(movement.x*movement.x + movement.z*movement.z);

      if (player.stepCounter >= 2.5f) {
        std::uniform_int_distribution<> distribution(0, PLAYER_FOOT_SOUND.size()-1);
        world.getPlayer().getComponent<SoundSource>().playSound(
          Environment::getDataDir() + PLAYER_FOOT_SOUND[distribution(Util::Rand)]);
        player.stepCounter -= 2.5f;
      }
    }
  };
}

std::string PlayerMoveTask::getName() {
  return "PlayerMoveTask";
}

} /* namespace radix */