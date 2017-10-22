#ifndef RADIX_SCREEN_TRIGGER_HPP
#define RADIX_SCREEN_TRIGGER_HPP

#include <string>

#include <radix/data/map/BaseTrigger.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

  class ScreenTrigger: public BaseTrigger {
  public:
    ScreenTrigger(const std::string &file);
    static const std::string TYPE;
    void addAction(Entity& trigger);
  private:
    std::string file;
  };

} /* namespace radix */

#endif /* RADIX_SCREEN_TRIGGER_HPP */

