#ifndef RADIX_TIME_DELTA_HPP
#define RADIX_TIME_DELTA_HPP

#include <cstdint>

namespace radix {

class TimeDelta final {
private:
  using Type = uint32_t;
  Type value;

public:
  constexpr TimeDelta(Type microsecs) : value(microsecs) {}
  constexpr operator uint32_t() const { return value; }

  static constexpr TimeDelta sec(Type t) { return TimeDelta(t*1000000); }
  constexpr Type sec() const { return value / 1000000; }
  constexpr Type sec_d() const { return value / 1000000.; }
  static constexpr TimeDelta msec(Type t) { return TimeDelta(t*1000); }
  constexpr Type msec() const { return value / 1000; }
  constexpr Type msec_d() const { return value / 1000.; }
  static constexpr TimeDelta usec(Type t) { return TimeDelta(t); }
  constexpr Type usec() const { return value; }
  constexpr Type usec_d() const { return (double) value; }
};

using TDelta = TimeDelta;

} /* namespace radix */

#endif /* RADIX_TIME_DELTA_HPP */