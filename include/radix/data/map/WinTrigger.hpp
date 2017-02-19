#ifndef RADIX_WIN_TRIGGER_HPP
#define RADIX_WIN_TRIGGER_HPP

#include <string>

#include <radix/data/map/BaseTrigger.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

  class WinTrigger: public BaseTrigger {
  public:
    static const std::string TYPE;
    void addAction(Entity& trigger);
  };

} /* namespace radix */

#endif /* RADIX_WIN_TRIGGER_HPP */
