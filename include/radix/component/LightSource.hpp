#ifndef COMPONENT_LIGHTSOURCE_HPP
#define COMPONENT_LIGHTSOURCE_HPP

#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

class LightSource : public Component {
public:
  bool enabled;
  Vector3f color;
  float distance;
  float energy;
  float specular;

  LightSource(Entity &ent) :
    Component(ent),
    enabled(true),
    color(1, 1, 1),
    distance(1),
    energy(10),
    specular(1),
    _uploaded(false) {
  }

  // Internal
  bool _uploaded;
};

} /* namespace radix */

#endif /* COMPONENT_LIGHTSOURCE_HPP */
