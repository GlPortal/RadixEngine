#ifndef RADIX_ENTITIES_LIGHT_SOURCE_HPP
#define RADIX_ENTITIES_LIGHT_SOURCE_HPP

#include <radix/entities/traits/LightSourceTrait.hpp>
#include <radix/Entity.hpp>

namespace radix {
namespace entities {

class LightSource :
    public virtual Entity,
    public LightSourceTrait {
public:
  using Entity::Entity;

  std::string fullClassName() const override {
    return "radix/entities/LightSource";
  }
  std::string className() const override {
    return "LightSource";
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_LIGHT_SOURCE_HPP */
