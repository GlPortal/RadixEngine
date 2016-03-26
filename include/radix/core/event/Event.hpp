#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

#include <radix/util/Hash.hpp>

namespace radix {

using StaticEventType = const uint32_t;
using EventType = uint32_t;
using StaticEventTypeName = const char *const;
using EventTypeName = std::string;

struct Event {
  constexpr static EventType TypeNameHash(StaticEventTypeName etn) {
    return (EventType) Hash32(etn);
  }

  virtual const EventType getType() const = 0;
  virtual const EventTypeName getTypeName() const = 0;
};

} /* namespace radix */

#endif /* EVENT_HPP */
