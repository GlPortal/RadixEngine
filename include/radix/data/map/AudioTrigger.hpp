#ifndef RADIX_AUDIO_TRIGGER_HPP
#define RADIX_AUDIO_TRIGGER_HPP

#include <string>

#include <radix/data/map/BaseTrigger.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

  class AudioTrigger: public BaseTrigger {
  private:
    std::string filePath;
    bool loop;
  public:
    static const std::string TYPE;
    AudioTrigger(std::string filePath);
    void setLoop(bool loop);
    void addAction(Entity& trigger);
  };

} /* namespace radix */

#endif /* RADIX_AUDIO_TRIGGER_HPP */
