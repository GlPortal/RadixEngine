#ifndef RADIX_COMPONENT_PLAYER_HPP
#define RADIX_COMPONENT_PLAYER_HPP

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

#include <radix/BaseGame.hpp>
#include <radix/component/Component.hpp>
#include <radix/component/Transform.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/Entity.hpp>
#include <radix/EntityManager.hpp>
#include <radix/physics/KinematicCharacterController.hpp>

namespace radix {

class ContactPlayerCallback : public btCollisionWorld::ContactResultCallback {
public:
  ContactPlayerCallback(BaseGame* game) : btCollisionWorld::ContactResultCallback(), game(game) { };

  BaseGame* game;

  virtual btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,
           int partId0, int index0,const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
    Entity* entity = (Entity*) colObj1Wrap->getCollisionObject()->getUserPointer(); //extract the second object since we don't care about the first

    if (entity->hasComponent<Trigger>()){
      Trigger& trigger = entity->getComponent<Trigger>();
      trigger.execute(game);
    }

    return 0;
  };
};

class Player : public Component {
public:
  std::shared_ptr<btConvexShape> shape;
  btPairCachingGhostObject *obj;
  KinematicCharacterController *controller;

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

  Quaternion getBaseHeadOrientation() const;
  Quaternion getHeadOrientation() const;
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_PLAYER_HPP */
