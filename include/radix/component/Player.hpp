#ifndef RADIX_COMPONENT_PLAYER_HPP
#define RADIX_COMPONENT_PLAYER_HPP

#include <memory>
#include <functional>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

#include <radix/PlayerTask.hpp>
#include <radix/BaseGame.hpp>
#include <radix/component/Component.hpp>
#include <radix/component/Transform.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/Entity.hpp>
#include <radix/EntityManager.hpp>
#include <radix/physics/KinematicCharacterController.hpp>

namespace radix {

class Player : public Component {
public:
  std::map<int, PlayerTask*> tasks;
  std::shared_ptr<btConvexShape> shape;
  btPairCachingGhostObject *obj;
  KinematicCharacterController *controller;
  Trigger* trigger;

  Vector3f velocity, headAngle;
  bool flying, noclip, frozen;
  float speed;
  float stepCounter;

  Player(Entity&);
  ~Player();

  const char* getName() const {
    return "Player";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver&);

  template<typename T>
  inline T& addTask() {
    static_assert(std::is_base_of<PlayerTask, T>::value, "T must be a PlayerTask");
    T *result = new T;
    if (!tasks.empty()) {
      int id = tasks.rbegin()->first + 1;
      result->id = id;
      tasks.insert(std::make_pair(id, result));
    } else {
      result->id = 0;
      tasks.insert(std::make_pair(0, result));
    }
    return *result;
  }
  void removeTask(int id);

  Quaternion getBaseHeadOrientation() const;
  Quaternion getHeadOrientation() const;
  inline void setHeadOrientation(Quaternion &quaternion) {
    headAngle = quaternion.toAero();
  }
};

class ContactPlayerCallback : public btCollisionWorld::ContactResultCallback {
public:
  ContactPlayerCallback(BaseGame &game) : btCollisionWorld::ContactResultCallback(), game(game) { };

  BaseGame &game;

  virtual btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,
                                   int partId0, int index0,const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
    Entity* playerEntity = (Entity*) colObj0Wrap->getCollisionObject()->getUserPointer();
    Entity* triggerEntity = (Entity*) colObj1Wrap->getCollisionObject()->getUserPointer();

    if (triggerEntity && playerEntity) {
      if (triggerEntity->hasComponent<Trigger>()) {
        Trigger &trigger = triggerEntity->getComponent<Trigger>();
        trigger.onUpdate(game);

        if (playerEntity->hasComponent<Player>()) {
          Player &player = playerEntity->getComponent<Player>();
          player.trigger = &trigger;
        }
      }
    }
    return 0;
  };
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_PLAYER_HPP */
