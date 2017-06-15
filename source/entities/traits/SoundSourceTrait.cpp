#include <radix/entities/traits/SoundSourceTrait.hpp>
#include <radix/SoundManager.hpp>

namespace radix {
namespace entities {

void SoundSourceTrait::playSound(const std::string &path) {
  SoundManager::playSound(path, *this);
}

} /* namespace entities */
} /* namespace radix */
