#ifndef RADIX_COMPONENT_TRIGGER_HPP
#define RADIX_COMPONENT_TRIGGER_HPP

#include <memory>
#include <functional>

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/Entity.hpp>
#include <radix/util/BulletGhostPairCallbacks.hpp>

namespace radix {

class BaseGame;

namespace entities {

class Trigger : public Entity {
private:
  util::BulletGhostPairCallbacks bulletGhostPairCallbacks;
  std::unique_ptr<btGhostObject> ghostObject;
  std::unique_ptr<btConvexShape> shape;

public:
  using Action = std::function<void(Trigger&)>;

  Action actionOnEnter;
  Action actionOnExit;
  Action actionOnMove;
  Action actionOnUpdate;

  Trigger(const CreationParams&);
  Trigger(const CreationParams&, const Transform&);
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

  virtual void setPosition(const Vector3f&) override;
  virtual void setOrientation(const Quaternion&) override;
  virtual void setScale(const Vector3f&) override;

  btGhostObject* getBulletGhostObject() const;

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
