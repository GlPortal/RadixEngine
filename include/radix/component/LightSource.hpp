#ifndef RADIX_COMPONENT_LIGHTSOURCE_HPP
#define RADIX_COMPONENT_LIGHTSOURCE_HPP

#include <radix/component/Component.hpp>

#include <serine/Archiver.hpp>

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
    specular(1) {
  }

  const char* getName() const {
    return "LightSource";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver &ar) {
    ar("enabled", enabled);
    ar("color", color);
    ar("distance", distance);
    ar("energy", energy);
    ar("specular", specular);
  }
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_LIGHTSOURCE_HPP */
