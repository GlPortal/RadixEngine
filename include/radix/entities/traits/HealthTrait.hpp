#ifndef RADIX_ENTITIES_HEALTH_TRAIT_HPP
#define RADIX_ENTITIES_HEALTH_TRAIT_HPP

#include <radix/entities/traits/Trait.hpp>

namespace radix {
namespace entities {

class HealthTrait : public Trait {
protected:
  HealthTrait();

public:
  radix_trait_declare("radix/entities/traits/", "Health")

  float maxHealth;
  float health;

  bool isAlive();
  void heal(float amount);
  void harm(float amount);
  void kill();
  void revive();
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_TRAITS_HEALTH_HPP */
