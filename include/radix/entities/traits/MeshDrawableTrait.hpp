#ifndef RADIX_ENTITIES_MESHDRAWABLE_TRAIT_HPP
#define RADIX_ENTITIES_MESHDRAWABLE_TRAIT_HPP

#include <radix/Entity.hpp>

#include <radix/data/material/Material.hpp>
#include <radix/data/model/Mesh.hpp>

namespace radix {
namespace entities {

class MeshDrawableTrait : public virtual Entity {
public:
  Mesh mesh;
  // TODO: MaterialDrawable?
  Material material;
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_MESHDRAWABLE_TRAIT_HPP */
