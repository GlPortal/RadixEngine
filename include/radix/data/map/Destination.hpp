#ifndef RADIX_DESTINATION_HPP
#define RADIX_DESTINATION_HPP

#include <radix/core/math/Quaternion.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

struct Destination {
  Vector3f position;
  Vector3f rotation;
  Quaternion orientation;
};

} /* namespace radix */

#endif /* RADIX_DESTINATION_HPP */
