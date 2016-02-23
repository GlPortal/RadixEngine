#ifndef COMPONENT_SOUNDLISTENER_HPP
#define COMPONENT_SOUNDLISTENER_HPP

#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

class SoundListener : public Component {
public:
  SoundListener(Entity &ent) :
    Component(ent) {}

  // TODO
};

} /* namespace radix */

#endif /* COMPONENT_SOUNDLISTENER_HPP */
