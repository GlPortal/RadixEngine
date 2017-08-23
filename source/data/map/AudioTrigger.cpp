#include <radix/data/map/AudioTrigger.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/core/state/GameState.hpp>
#include <radix/env/Environment.hpp>
#include <radix/SoundManager.hpp>

using namespace std;

namespace radix {

const std::string AudioTrigger::TYPE = "audio";

AudioTrigger::AudioTrigger(std::string filePath) {
  this->filePath = filePath;
}

void AudioTrigger::setLoop(bool loop) {
  this->loop = loop;
}

void AudioTrigger::addAction(Entity &ent) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(ent);
  std::string datadir  = Environment::getDataDir();
  std::string fileName = datadir + "/audio/" + this->filePath;
  trigger.setActionOnEnter([fileName] (entities::Trigger &trigger) {
    if (!SoundManager::isPlaying(fileName)) {
      SoundManager::playMusic(fileName);
    }
  });
}

} /* namespace radix */
