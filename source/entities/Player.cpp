#include <radix/entities/Player.hpp>

#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>

#include <radix/env/Environment.hpp>
#include <radix/simulation/Physics.hpp>
#include <radix/World.hpp>
#include <radix/BaseGame.hpp>

#include <cmath>
#include <array>
#include <cstdlib>
#include <string>

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
  stepCounter(0),
  attemptJump(false) {

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
  if (frozen) {
    return;
  }

  headAngle.attitude -= headingChange.y;
  headAngle.attitude = Math::clamp(headAngle.attitude, rad(-89.99f), rad(89.99f));
  headAngle.heading -= headingChange.x;
  headAngle.tilt *= 0.8f;

  float rot = headAngle.heading;
  privSetPosition(obj->getWorldTransform().getOrigin());

  if (attemptJump and controller->canJump()) {
    std::uniform_int_distribution<> dis(0, PLAYER_JUMP_SOUND.size()-1);
    playSound(Environment::getDataDir() + PLAYER_JUMP_SOUND[dis(Util::Rand)]);
    controller->jump();
  }

  if (movement != Vector3f::ZERO) {
    if (trigger) {
      trigger->actionOnMove(*trigger);
    }
  }

  Matrix4f rotationMatrix;
  rotationMatrix.rotate(rot, 0.0f, 1.0f, 0.0f);

  Vector3f newMovement = rotationMatrix * movement;
  newMovement *= RUNNING_SPEED;
  
  controller->setWalkDirection(newMovement);

  if (controller->onGround()) {
    stepCounter += std::sqrt(newMovement.x*newMovement.x + newMovement.z*newMovement.z);

    if (stepCounter >= 2.5f) {
      std::uniform_int_distribution<> distribution(0, PLAYER_FOOT_SOUND.size()-1);
      playSound(Environment::getDataDir() + PLAYER_FOOT_SOUND[distribution(Util::Rand)]);
      stepCounter -= 2.5f;
    }
  }
  attemptJump = false;
  trigger = nullptr;
}

void Player::jump() {
  attemptJump = true;
}

void Player::move(const Vector2f &move) {
  movement.x = move.x;
  movement.z = move.y;
}

void Player::moveX(const float &moveX) {
  movement.x = moveX;
}

void Player::moveY(const float &moveY) {
  movement.z = moveY;
}

void Player::changeHeading(const Vector2f &lookVector) {
	headingChange.x = lookVector.x;
	headingChange.y = lookVector.y;
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
