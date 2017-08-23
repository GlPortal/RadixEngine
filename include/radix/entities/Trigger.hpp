#ifndef RADIX_COMPONENT_TRIGGER_HPP
#define RADIX_COMPONENT_TRIGGER_HPP

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/Entity.hpp>

#include <memory>
#include <functional>
#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace radix {

class BaseGame;

namespace entities {

class Trigger : public Entity {
private:
  btGhostObject *ghostObject;
public:
  btGhostObject *getBulletGhostObject();
  // TODO: replace BaseGame& with Trigger&, because it prevents you from distinguishing
  // on what Trigger in which World the event happens when using the same Action
  using Action = std::function<void(BaseGame&)>;

  Action actionOnEnter;
  Action actionOnExit;
  Action actionOnMove;
  Action actionOnUpdate;

  EventDispatcher::CallbackHolder callbackOnEnter, callbackOnExit;

  std::shared_ptr<btConvexShape> shape;

  Trigger(const CreationParams&);
  ~Trigger();

  const char* getName() const {
    return "Trigger";
  }

  void setActionOnExit(Action action);
  void setActionOnEnter(Action action);
  void setActionOnMove(Action action);
  void setActionOnUpdate(Action action);

  void serialize(serine::Archiver &ar) {
  }

  void onEnter(BaseGame& game) { actionOnEnter(game); }
  void onExit(BaseGame& game) { actionOnExit(game); }
  void onMove(BaseGame& game) { actionOnMove(game); }
  void onUpdate(BaseGame& game) { actionOnUpdate(game); }

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
