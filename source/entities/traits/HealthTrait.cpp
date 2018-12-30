#include <radix/entities/traits/HealthTrait.hpp>

#include <serine/Archiver.hpp>

#include <radix/core/math/Math.hpp>
#include <ciso646>

namespace radix {
namespace entities {

HealthTrait::HealthTrait() :
  maxHealth(1.f),
  health(maxHealth) {
}

bool HealthTrait::isAlive() {
  return health > 0;
}

void HealthTrait::heal(float amount) {
  health = Math::clamp(health + amount, 0.f, maxHealth);
}

void HealthTrait::harm(float amount) {
  health = Math::clamp(health - amount, 0.f, maxHealth);
}

void HealthTrait::kill() {
  health = 0;
}

void HealthTrait::revive() {
  if (not isAlive()) {
    health = 1.f;
  }
}

} /* namespace entities */
} /* namespace radix */
