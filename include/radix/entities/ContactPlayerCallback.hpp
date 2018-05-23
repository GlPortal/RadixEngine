#ifndef RADIX_ENTITIES_CONTACT_PLAYER_CALLBACK_HPP
#define RADIX_ENTITIES_CONTACT_PLAYER_CALLBACK_HPP

namespace radix {
namespace entities {

class Player;

class ContactPlayerCallback : public btCollisionWorld::ContactResultCallback {
public:
  virtual btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,
                                   int partId0, int index0,const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
    Entity* playerEntity = util::getBtPtrInfo(colObj0Wrap->getCollisionObject()).entity;
    Entity* triggerEntity = util::getBtPtrInfo(colObj1Wrap->getCollisionObject()).entity;

    if (triggerEntity && playerEntity) {
      Trigger *trigger = dynamic_cast<Trigger*>(triggerEntity);
      if (!trigger) {
        return 0;
      }
      trigger->onUpdate();

      Player *player = dynamic_cast<Player*>(playerEntity);
      if (player) {
        player->trigger = trigger;
      }
    }
    return 0;
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_CONTACT_PLAYER_CALLBACK_HPP */
