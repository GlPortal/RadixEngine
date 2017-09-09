#ifndef RADIX_COMPONENT_TRIGGER_HPP
#define RADIX_COMPONENT_TRIGGER_HPP

#include <memory>
#include <functional>

#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/Entity.hpp>
#include <radix/util/BulletGhostPairCallbacks.hpp>

namespace radix {

class BaseGame;

namespace entities {

class Trigger : public Entity {
private:
  util::BulletGhostPairCallbacks m_btGpCallbacks;
  btGhostObject *ghostObject;

public:
  btGhostObject *getBulletGhostObject();
  // TODO: replace BaseGame& with Trigger&, because it prevents you from distinguishing
  // on what Trigger in which World the event happens when using the same Action
  using Action = std::function<void(Trigger&)>;

  Action actionOnEnter;
  Action actionOnExit;
  Action actionOnMove;
  Action actionOnUpdate;

  std::shared_ptr<btConvexShape> shape;

  Trigger(const CreationParams&);
  ~Trigger();

  void setActionOnExit(Action action);
  void setActionOnEnter(Action action);
  void setActionOnMove(Action action);
  void setActionOnUpdate(Action action);

  void serialize(serine::Archiver &ar) {
  }

  void onEnter() { actionOnEnter(*this); }
  void onExit() { actionOnExit(*this); }
  void onMove() { actionOnMove(*this); }
  void onUpdate() { actionOnUpdate(*this); }

  std::string fullClassName() const override {
    return "radix/entities/Trigger";
  }
  std::string className() const override {
    return "Trigger";
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_COMPONENT_TRIGGER_HPP */
