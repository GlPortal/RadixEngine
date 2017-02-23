#include <radix/data/map/AudioTrigger.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/Health.hpp>
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

  void AudioTrigger::addAction(Entity& trigger) {
    std::function<void(BaseGame&)> action;
    std::string datadir  = Environment::getDataDir();
    std::string fileName = datadir + "/audio/" + this->filePath;
    action = [fileName] (BaseGame &game) {
      if (!SoundManager::isPlaying(fileName)) {
        SoundManager::playMusic(fileName);
      }
    };
    trigger.getComponent<Trigger>().setActionOnEnter(action);
  }
} /* namespace radix */
