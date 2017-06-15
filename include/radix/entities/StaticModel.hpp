#ifndef RADIX_ENTITIES_STATIC_MODEL_HPP
#define RADIX_ENTITIES_STATIC_MODEL_HPP

#include <radix/entities/traits/MeshDrawableTrait.hpp>
#include <radix/entities/traits/RigidBodyTrait.hpp>
#include <radix/Entity.hpp>

namespace radix {
namespace entities {

class StaticModel :
    public virtual Entity,
    public MeshDrawableTrait,
    public RigidBodyTrait {
public:
  StaticModel(const CreationParams&);
  ~StaticModel();

  std::string fullClassName() const override {
    return "radix/entities/StaticModel";
  }
  std::string className() const override {
    return "StaticModel";
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_STATIC_MODEL_HPP */
