#ifndef RADIX_COMPONENT_TRIGGER_HPP
#define RADIX_COMPONENT_TRIGGER_HPP

#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>

#include <functional>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace radix {

class Trigger : public Component {
public:
  using Action = std::function<void()>;

  btGhostObject *obj;
  // duk_c_function script;
  // TODO: EntityFilter filter;

  Trigger(Entity &ent) :
    Component(ent) {}

  const char* getName() const {
    return "Trigger";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver &ar) {
    // ...
  }

  void execute() { Action(); };
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_TRIGGER_HPP */
