#ifndef RADIX_DEATH_TRIGGER_HPP
#define RADIX_DEATH_TRIGGER_HPP

#include <string>

#include <radix/data/map/BaseTrigger.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/Entity.hpp>

namespace radix {

  class DeathTrigger: public BaseTrigger {
  public:
    static const std::string TYPE;
    void addAction(Entity& trigger);
  };

} /* namespace radix */

#endif /* RADIX_DEATH_TRIGGER_HPP */
