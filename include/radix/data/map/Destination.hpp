#ifndef RADIX_DESTINATION_HPP
#define RADIX_DESTINATION_HPP

namespace radix {

struct Vector3f;
struct Quaternion;

struct Destination {
  Vector3f position;
  Vector3f rotation;
  Quaternion orientation;
};

} /* namespace radix */

#endif /* RADIX_DESTINATION_HPP */
