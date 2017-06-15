#ifndef RADIX_ENTITIES_SOUNDSOURCE_TRAIT_HPP
#define RADIX_ENTITIES_SOUNDSOURCE_TRAIT_HPP

#include <radix/Entity.hpp>

namespace radix {
namespace entities {

class SoundSourceTrait : public virtual Entity {
public:
  void playSound(const std::string &path);
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_SOUNDSOURCE_TRAIT_HPP */
