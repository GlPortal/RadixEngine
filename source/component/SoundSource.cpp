#include <radix/component/SoundSource.hpp>
#include <radix/SoundManager.hpp>

namespace glPortal {

void SoundSource::playSound(const std::string &path) {
  SoundManager::playSound(path, entity);
}

} /* namespace glPortal */