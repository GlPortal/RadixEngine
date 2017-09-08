#ifndef RADIX_ENTITIES_REFERENCE_POINT_HPP
#define RADIX_ENTITIES_REFERENCE_POINT_HPP

#include <radix/Entity.hpp>

namespace radix {
namespace entities {

class ReferencePoint :
    public virtual Entity {
public:
  using Entity::Entity;

  std::string fullClassName() const override {
    return "radix/entities/ReferencePoint";
  }
  std::string className() const override {
    return "ReferencePoint";
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_REFERENCE_POINT_HPP */
