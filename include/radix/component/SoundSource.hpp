#ifndef RADIX_COMPONENT_SOUNDSOURCE_HPP
#define RADIX_COMPONENT_SOUNDSOURCE_HPP

#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

class SoundSource : public Component {
public:
  SoundSource(Entity &ent) :
    Component(ent) {}

  const char* getName() const {
    return "SoundSource";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver &ar) {

  }

  // TODO

  void playSound(const std::string &path);
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_SOUNDSOURCE_HPP */
