#ifndef RADIX_ENTITIES_LIGHTSOURCE_TRAIT_HPP
#define RADIX_ENTITIES_LIGHTSOURCE_TRAIT_HPP

#include <radix/entities/traits/Trait.hpp>

#include <radix/core/math/Vector3f.hpp>

namespace radix {
namespace entities {

class LightSourceTrait : public Trait {
public:
  bool enabled;
  Vector3f color;
  float distance;
  float energy;
  float specular;

protected:
  LightSourceTrait() :
    enabled(true),
    color(1, 1, 1),
    distance(1),
    energy(10),
    specular(1) {
  }

public:
  radix_trait_declare("radix/entities/traits/", "LightSource")
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_TRAITS_LIGHTSOURCE_TRAIT_HPP */
