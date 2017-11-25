#ifndef RADIX_ENTITIES_SOUNDSOURCE_TRAIT_HPP
#define RADIX_ENTITIES_SOUNDSOURCE_TRAIT_HPP

#include <radix/entities/traits/Trait.hpp>

namespace radix {
namespace entities {

class SoundSourceTrait : public Trait {
public:
  radix_trait_declare("radix/entities/traits/", "SoundSource")

  void playSound(const std::string &path);
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_SOUNDSOURCE_TRAIT_HPP */
