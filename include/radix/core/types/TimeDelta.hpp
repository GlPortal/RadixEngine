#ifndef RADIX_TIME_DELTA_HPP
#define RADIX_TIME_DELTA_HPP

#include <cstdint>

namespace radix {

class TimeDelta final {
private:
  using Type = uint32_t;
  Type value;

  constexpr TimeDelta(Type microsecs) : value(microsecs) {}
  constexpr TimeDelta(double microsecs) : value(static_cast<Type>(microsecs)) {}
  constexpr TimeDelta(int microsecs) : value(static_cast<Type>(microsecs)) {}

public:
  constexpr TimeDelta() : value(0) {}
  constexpr operator uint32_t() const { return value; }

  static constexpr TimeDelta sec(double t) { return TimeDelta(t*1000000); }
  static constexpr TimeDelta sec(int t) { return TimeDelta(t*1000000); }
  static constexpr TimeDelta sec(Type t) { return TimeDelta(t*1000000); }
  constexpr double sec() const { return value / 1000000.; }
  static constexpr TimeDelta msec(double t) { return TimeDelta(t*1000); }
  static constexpr TimeDelta msec(int t) { return TimeDelta(t*1000); }
  static constexpr TimeDelta msec(Type t) { return TimeDelta(t*1000); }
  constexpr double msec() const { return value / 1000.; }
  static constexpr TimeDelta usec(double t) { return TimeDelta(t); }
  static constexpr TimeDelta usec(int t) { return TimeDelta(t); }
  static constexpr TimeDelta usec(Type t) { return TimeDelta(t); }
  constexpr Type usec() const { return value; }
  constexpr double usec_d() const { return static_cast<double>(value); }
};

using TDelta = TimeDelta;

} /* namespace radix */

#endif /* RADIX_TIME_DELTA_HPP */
