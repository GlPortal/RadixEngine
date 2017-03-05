#ifndef RADIX_TELEPORT_TRIGGER_HPP
#define RADIX_TELEPORT_TRIGGER_HPP

#include <string>

#include <radix/data/map/BaseTrigger.hpp>

namespace radix {

class TeleportTrigger : public BaseTrigger {
public:
  TeleportTrigger(std::string& destination);
  static const std::string TYPE;
  std::string destination;
  void addAction(Entity& trigger);
};

} /* namespace radix */

#endif /* RADIX_TELEPORT_TRIGGER_HPP */
