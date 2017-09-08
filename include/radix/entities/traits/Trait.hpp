#ifndef RADIX_ENTITIES_TRAIT_HPP
#define RADIX_ENTITIES_TRAIT_HPP

#include <radix/Entity.hpp>

namespace radix {
namespace entities {

class Trait : public virtual Entity {
protected:
  Trait() = default;
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_TRAIT_HPP */
