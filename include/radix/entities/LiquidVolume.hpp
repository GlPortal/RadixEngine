#ifndef RADIX_ENTITIES_LIQUID_VOLUME_HPP
#define RADIX_ENTITIES_LIQUID_VOLUME_HPP

#include <radix/entities/traits/MeshDrawableTrait.hpp>
#include <radix/Entity.hpp>

namespace radix {
namespace entities {

class LiquidVolume :
    public virtual Entity,
    public MeshDrawableTrait {
public:
  using Entity::Entity;

  std::string fullClassName() const override {
    return "radix/entities/LiquidVolume";
  }
  std::string className() const override {
    return "LiquidVolume";
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_LIQUID_VOLUME_HPP */
