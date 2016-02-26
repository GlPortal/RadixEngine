#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

namespace radix {

using StaticEventType = const char*const;
using EventType = std::string;

struct Event {
  virtual const EventType getType() const = 0;
};

} /* namespace radix */

#endif /* EVENT_HPP */
