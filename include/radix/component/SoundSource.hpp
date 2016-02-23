#ifndef COMPONENT_SOUNDSOURCE_HPP
#define COMPONENT_SOUNDSOURCE_HPP

#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

class SoundSource : public Component {
public:
  SoundSource(Entity &ent) :
    Component(ent) {}

  // TODO

  void playSound(const std::string &path);
};

} /* namespace radix */

#endif /* COMPONENT_SOUNDSOURCE_HPP */
