#include <radix/core/math/Vector2i.hpp>

#include <sstream>

namespace radix {

const Vector2i Vector2i::ZERO(0, 0);
const Vector2i Vector2i::UP(0, 1);

std::string Vector2i::str() const {
  std::stringstream ss;
  ss << "(" << x << ", " << y << ")";
  return ss.str();
}

} /* namespace radix */
