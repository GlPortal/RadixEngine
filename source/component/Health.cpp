#include <radix/component/Health.hpp>

#include <serine/Archiver.hpp>

#include <radix/core/math/Math.hpp>

#include <ciso646>

namespace radix {

Health::Health(Entity &ent) :
  Component(ent), maxHealth(1.f), health(maxHealth) {
}

void Health::serialize(serine::Archiver &ar) {
  ar("maxHealth", maxHealth);
  ar("health", health);
}

bool Health::isAlive() {
  return health > 0;
}

void Health::heal(float amount) {
  health = Math::clamp(health + amount, 0.f, maxHealth);
}

void Health::harm(float amount) {
  health = Math::clamp(health - amount, 0.f, maxHealth);
}

void Health::kill() {
  health = 0;
}

void Health::revive() {
  if (not isAlive()) {
    health = 1.f;
  }
}

} /* namespace radix */
