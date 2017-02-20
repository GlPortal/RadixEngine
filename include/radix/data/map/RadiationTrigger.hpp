#ifndef RADIX_RADIATION_TRIGGER_HPP
#define RADIX_RADIATION_TRIGGER_HPP

#include <string>

#include <radix/data/map/BaseTrigger.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

  class RadiationTrigger: public BaseTrigger {
  public:
    static const std::string TYPE;
    void addAction(Entity& trigger);
  };

} /* namespace radix */

#endif /* RADIX_RADIATION_TRIGGER_HPP */
