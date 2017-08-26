#ifndef RADIX_CORE_EVENT_EVENT_HPP
#define RADIX_CORE_EVENT_EVENT_HPP

#include <string>

#include <radix/util/Hash.hpp>

#define radix_event_declare(TN) static constexpr StaticEventTypeName TypeName = TN; \
    const EventTypeName getTypeName() const override { \
      return TypeName; \
    } \
    static constexpr StaticEventType Type = TypeNameHash(TypeName); \
    const EventType getType() const override { \
      return Type; \
    }

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
  virtual std::string debugStringRepr() const { return ""; }
};

} /* namespace radix */

#endif /* RADIX_CORE_EVENT_EVENT_HPP */
