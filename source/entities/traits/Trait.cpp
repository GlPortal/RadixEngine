#include <radix/entities/traits/Trait.hpp>

namespace radix {
namespace entities {

Trait::Trait() {
  m_traits.emplace_back(this);
}

} /* namespace entities */
} /* namespace radix */
