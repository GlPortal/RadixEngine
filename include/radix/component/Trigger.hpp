#ifndef RADIX_COMPONENT_TRIGGER_HPP
#define RADIX_COMPONENT_TRIGGER_HPP

#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

class Trigger : public Component {
public:
  // FIXME: no types, scripts
  std::string type;
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

  void onEnter(Entity&);
  void onMove(Entity&);
  void onExit(Entity&);
  void onTouchStart(Entity&);
  void onTouchEnd(Entity&);
  void onUseStart(Entity&);
  void onUseEnd(Entity&);
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_TRIGGER_HPP */
