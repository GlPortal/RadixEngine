#include <radix/core/math/Vector2ui.hpp>

#include <cmath>
#include <sstream>

namespace radix {

  const Vector2ui Vector2ui::ZERO(0, 0);
  const Vector2ui Vector2ui::UP(0, 1);

  /* Core */
  unsigned int Vector2ui::length() const {
    return sqrt(x * x + y * y);
  }

  std::string Vector2ui::str() const {
    std::stringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
  }

} /* namespace radix */
