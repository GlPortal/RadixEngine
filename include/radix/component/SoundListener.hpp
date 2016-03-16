#ifndef RADIX_COMPONENT_SOUNDLISTENER_HPP
#define RADIX_COMPONENT_SOUNDLISTENER_HPP

#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

class SoundListener : public Component {
public:
  SoundListener(Entity &ent) :
    Component(ent) {}

  const char* getName() const {
    return "SoundSource";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver&) {
    // TODO
  }

  // TODO
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_SOUNDLISTENER_HPP */
