#include <radix/component/SoundSource.hpp>
#include <radix/SoundManager.hpp>

namespace radix {

void SoundSource::playSound(const std::string &path) {
  SoundManager::playSound(path, entity);
}

} /* namespace radix */