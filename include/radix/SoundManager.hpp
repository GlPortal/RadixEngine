#ifndef RADIX_SOUNDMANAGER_HPP
#define RADIX_SOUNDMANAGER_HPP

#include <map>
#include <string>

#include <SDL2/SDL_mixer.h>

#include <radix/Entity.hpp>

namespace radix {

class SoundManager {
public:
  struct SoundInfo {
    Mix_Chunk *chunk;
  };

  static void init();
  static void reload();
  static void destroy();

  static void playMusic(const std::string &filename);
  static void playSound(const std::string &filename, const Entity &source);

  static void update(const Entity &listener);

private:
  static bool isInitialized, isDisabled;

  static Mix_Music *music;
  static std::map<int, SoundInfo> sounds;
};

} /* namespace radix */

#endif /* RADIX_SOUNDMANAGER_HPP */
