#ifndef RADIX_COMPONENT_TRIGGER_HPP
#define RADIX_COMPONENT_TRIGGER_HPP

#include <radix/BaseGame.hpp>
#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/Entity.hpp>

#include <memory>
#include <functional>
#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace radix {

class Trigger : public Component {
private:
  btGhostObject *ghostObject;
public:
  btGhostObject *getBulletGhostObject();
  using Action = std::function<void(BaseGame&)>;

  Action actionOnEnter;
  Action actionOnExit;
  Action actionOnMove;
  Action actionOnUpdate;

  EventDispatcher::CallbackHolder callbackOnEnter, callbackOnExit;

  std::shared_ptr<btConvexShape> shape;

  Trigger(Entity &ent);
  ~Trigger();

  const char* getName() const {
    return "Trigger";
  }

  void setActionOnExit(Action action);
  void setActionOnEnter(Action action);
  void setActionOnMove(Action action);
  void setActionOnUpdate(Action action);

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver &ar) {
  }

  void onEnter(BaseGame& game) { actionOnEnter(game); };
  void onExit(BaseGame& game) { actionOnExit(game); };
  void onMove(BaseGame& game) { actionOnMove(game); };
  void onUpdate(BaseGame& game) { actionOnUpdate(game); };
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_TRIGGER_HPP */
