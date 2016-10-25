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
#include <radix/World.hpp>

namespace radix {

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

PlayerSystem::PlayerSystem(World &w) :
  System(w) {
}

PlayerSystem::~PlayerSystem() {
}

void PlayerSystem::mouseLook(Entity &entity) {
  Player &plr = entity.getComponent<Player>();
  if (plr.frozen) {
    return;
  }
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);

  // Apply mouse movement to view
  //Vector3f &rotation = entity.getComponent<Transform>().rotation;
  if (world.getConfig().isLoaded()) {
    plr.headAngle.attitude -= rad(mousedy * world.getConfig().getSensitivity());
    plr.headAngle.heading  -= rad(mousedx * world.getConfig().getSensitivity());
  } else {
    plr.headAngle.attitude -= rad(mousedy * 0.30);
    plr.headAngle.heading  -= rad(mousedx * 0.30);
  }
  plr.headAngle.tilt *= 0.8;

  // Restrict rotation in horizontal axis
  plr.headAngle.attitude = Math::clamp(plr.headAngle.attitude, rad(-89.99), rad(89.99));
}

void PlayerSystem::move(Entity &entity, TDelta dtime) {
  (void) dtime;
  Player &plr = entity.getComponent<Player>();
  if (plr.frozen) {
    return;
  }
  InputSource &input = world.input;
  bool movingFwd     = input.isKeyDown(SDL_SCANCODE_W) or input.isKeyDown(SDL_SCANCODE_UP),
       movingBack    = input.isKeyDown(SDL_SCANCODE_S) or input.isKeyDown(SDL_SCANCODE_DOWN),
       strafingLeft  = input.isKeyDown(SDL_SCANCODE_A) or input.isKeyDown(SDL_SCANCODE_LEFT),
       strafingRight = input.isKeyDown(SDL_SCANCODE_D) or input.isKeyDown(SDL_SCANCODE_RIGHT),
       jumping       = input.isKeyDown(SDL_SCANCODE_SPACE) or
                       input.isKeyDown(SDL_SCANCODE_BACKSPACE);
  float rot = plr.headAngle.heading;
  Vector3f movement;
  KinematicCharacterController &ctrl = *entity.getComponent<Player>().controller;
  Transform &plrTform = entity.getComponent<Transform>();
  plrTform.privSetPosition(entity.getComponent<Player>().obj->getWorldTransform().getOrigin());

  if (jumping and ctrl.canJump()) {
    std::uniform_int_distribution<> dis(0, PLAYER_JUMP_SOUND.size()-1);
      entity.getComponent<SoundSource>().playSound(
        Environment::getDataDir() + PLAYER_JUMP_SOUND[dis(Util::Rand)]);
    ctrl.jump();
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
  ctrl.setWalkDirection(movement);

  if (ctrl.onGround()) {
    plr.stepCounter += std::sqrt(movement.x*movement.x + movement.z*movement.z);

    if (plr.stepCounter >= 2.5f) {
      std::uniform_int_distribution<> dis(0, PLAYER_FOOT_SOUND.size()-1);
      entity.getComponent<SoundSource>().playSound(
        Environment::getDataDir() + PLAYER_FOOT_SOUND[dis(Util::Rand)]);
      plr.stepCounter -= 2.5f;
    }
  }
#if 0
  float rot = rotation.y;
  if (entity.getComponent<Health>().isAlive()) {
    Vector3f tmpVel;
    double yMult = flying ? cos(rotation.x) : 1;
    if (movingFwd) {
      tmpVel.x -= sin(rot) * yMult;
      tmpVel.z -= cos(rot) * yMult;
      if (flying) {
        tmpVel.y += sin(rotation.x);
      }
    }
    if (movingBack) {
      tmpVel.x += sin(rot) * yMult;
      tmpVel.z += cos(rot) * yMult;
      if (flying) {
        tmpVel.y -= sin(rotation.x);
      }
    }
#endif
}

bool PlayerSystem::runsBefore(const System &sys) {
  return sys.getTypeId() == System::getTypeId<PhysicsSystem>();
}

void PlayerSystem::update(TDelta dtime) {
  mouseLook(world.getPlayer());
  move(world.getPlayer(), dtime);
}

} /* namespace radix */
