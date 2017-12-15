#ifndef RADIX_ENTITIES_SOUND_PHYSICAL_MEDIUM_TRAIT_HPP
#define RADIX_ENTITIES_SOUND_PHYSICAL_MEDIUM_TRAIT_HPP

#include <radix/entities/traits/Trait.hpp>

namespace radix {
namespace entities {

class SoundPhysicalMediumTrait : public Trait {
public:
  /* TODO
  double speedOfSound;
  */

  radix_trait_declare("radix/entities/traits/", "SoundPhysicalMedium")
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_SOUND_PHYSICAL_MEDIUM_TRAIT_HPP */
