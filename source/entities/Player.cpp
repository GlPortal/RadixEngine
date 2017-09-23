#include <radix/entities/Player.hpp>

#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>

#include <SDL2/SDL_scancode.h>

#include <radix/env/Environment.hpp>
#include <radix/input/InputSource.hpp>
#include <radix/simulation/Physics.hpp>
#include <radix/World.hpp>

namespace radix {
namespace entities {

static const Vector3f PLAYER_SIZE(0.5, 1, 0.5);

static constexpr float RUNNING_SPEED = 0.1f;
static constexpr float JUMP_SPEED = 0.15f;
static constexpr float HURT_VELOCITY = 0.18f;

static const std::array<const std::string, 2> PLAYER_PANTING_SOUND = {{
  "/audio/sfx/character/fem_panting_1.ogg",
  "/audio/sfx/character/fem_panting_2.ogg"
}};

static const std::array<const std::string, 2> PLAYER_JUMP_SOUND = {{
  "/audio/sfx/character/fem_jump_1.ogg",
  "/audio/sfx/character/fem_jump_2.ogg"
}};

static const std::array<const std::string, 2> PLAYER_FALL_SOUND = {{
  "/audio/sfx/character/fem_fall_1.ogg",
  "/audio/sfx/character/fem_fall_2.ogg"
}};

static const std::array<const std::string, 6> PLAYER_FOOT_SOUND = {{
  "/audio/sfx/character/fem_foot_1.ogg",
  "/audio/sfx/character/fem_foot_2.ogg",
  "/audio/sfx/character/fem_foot_3.ogg",
  "/audio/sfx/character/fem_foot_4.ogg",
  "/audio/sfx/character/fem_foot_5.ogg",
  "/audio/sfx/character/fem_foot_6.ogg"
}};

Player::Player(const CreationParams &cp) :
  Entity(cp),
  m_btGpCallbacks(this),
  flying(false),
  noclip(false),
  frozen(false),
  stepCounter(0) {

  setScale(PLAYER_SIZE);

  obj = new btPairCachingGhostObject;
  obj->setWorldTransform(btTransform(getOrientation(), getPosition()));
  shape = std::make_shared<btCapsuleShape>(.4, 1);
  obj->setCollisionShape(shape.get());
  obj->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT |
                         btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  obj->setUserPointer(&m_btGpCallbacks);
  obj->setUserIndex(id);
  controller = new KinematicCharacterController(obj, shape.get(), 0.35);
  auto &physWorld = world.simulations.findFirstOfType<simulation::Physics>().getPhysicsWorld();
  physWorld.addCollisionObject(obj,
      btBroadphaseProxy::CharacterFilter,
      btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::DebrisFilter);
  physWorld.addAction(controller);
}

Player::~Player() {
  auto &physWorld = world.simulations.findFirstOfType<simulation::Physics>().getPhysicsWorld();
  physWorld.removeAction(controller);
  physWorld.removeCollisionObject(obj);
  delete controller;
  delete obj;
}

void Player::tick(TDelta dtime) {
  (void) dtime;
  trigger = nullptr;
  if (frozen) {
    return;
  }

  // Head rotation
  // TODO: don't use SDL directly
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);
  // Apply mouse movement to view
  if (world.getConfig().isLoaded()) {
    headAngle.attitude -= rad(mousedy * world.getConfig().getSensitivity());
    headAngle.heading  -= rad(mousedx * world.getConfig().getSensitivity());
  } else {
    headAngle.attitude -= rad(mousedy * 0.30);
    headAngle.heading  -= rad(mousedx * 0.30);
  }
  headAngle.tilt *= 0.8;

  // Restrict rotation in horizontal axis
  headAngle.attitude = Math::clamp(headAngle.attitude, rad(-89.99), rad(89.99));

  InputSource &input = *world.input;
  bool movingFwd     = input.isKeyDown(SDL_SCANCODE_W) or input.isKeyDown(SDL_SCANCODE_UP),
       movingBack    = input.isKeyDown(SDL_SCANCODE_S) or input.isKeyDown(SDL_SCANCODE_DOWN),
       strafingLeft  = input.isKeyDown(SDL_SCANCODE_A) or input.isKeyDown(SDL_SCANCODE_LEFT),
       strafingRight = input.isKeyDown(SDL_SCANCODE_D) or input.isKeyDown(SDL_SCANCODE_RIGHT),
       jumping       = input.isKeyDown(SDL_SCANCODE_SPACE) or
                       input.isKeyDown(SDL_SCANCODE_BACKSPACE);
  float rot = headAngle.heading;
  Vector3f movement;
  privSetPosition(obj->getWorldTransform().getOrigin());

  if (jumping and controller->canJump()) {
    std::uniform_int_distribution<> dis(0, PLAYER_JUMP_SOUND.size()-1);
    playSound(Environment::getDataDir() + PLAYER_JUMP_SOUND[dis(Util::Rand)]);
    controller->jump();
  }

  /*if (movingFwd || movingBack || strafingLeft || strafingRight) {
    if (trigger) {
      trigger->actionOnMove(game);
    }
  }*/

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
  controller->setWalkDirection(movement);

  if (controller->onGround()) {
    stepCounter += std::sqrt(movement.x*movement.x + movement.z*movement.z);

    if (stepCounter >= 2.5f) {
      std::uniform_int_distribution<> distribution(0, PLAYER_FOOT_SOUND.size()-1);
      playSound(Environment::getDataDir() + PLAYER_FOOT_SOUND[distribution(Util::Rand)]);
      stepCounter -= 2.5f;
    }
  }
}

Quaternion Player::getBaseHeadOrientation() const {
  return Quaternion().fromAero(headAngle);
}

Quaternion Player::getHeadOrientation() const {
  return Quaternion().fromAero(headAngle);
}

void Player::setPosition(const Vector3f &pos) {
  position = pos;
  controller->warp(pos);
}

} /* namespace entities */
} /* namespace radix */
