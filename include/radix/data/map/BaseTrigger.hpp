#ifndef RADIX_BASE_TRIGGER_HPP
#define RADIX_BASE_TRIGGER_HPP

#include <string>

#include <radix/Entity.hpp>

namespace radix {

class BaseTrigger {
public:
  virtual void addAction(Entity& trigger) {}
};

} /* namespace radix */

#endif /* RADIX_BASE_TRIGGER_HPP */
