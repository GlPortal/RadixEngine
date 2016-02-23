#ifndef MUSIC_OBSERVER_HPP
#define MUSIC_OBSERVER_HPP

#include <random>

#include <radix/core/event/Observer.hpp>
#include <radix/SoundManager.hpp>
#include <radix/env/Environment.hpp>

namespace radix {

const std::array<const std::string, 3> MUSIC_PLAYLIST = {
  "/audio/music/track1.ogg",
  "/audio/music/track2.ogg",
  "/audio/music/track3.ogg"
};

class MusicObserver : public Observer{
private:
  std::mt19937 generator;
public:
  void loadMap();
};

} /* namespace radix */

#endif /* MUSIC_OBSERVER_HPP */
