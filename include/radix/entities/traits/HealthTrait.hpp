#ifndef RADIX_ENTITIES_HEALTH_TRAIT_HPP
#define RADIX_ENTITIES_HEALTH_TRAIT_HPP

#include <radix/Entity.hpp>

namespace radix {
namespace entities {

class HealthTrait : public virtual Entity {
public:
  float maxHealth;
  float health;

  HealthTrait();

  bool isAlive();
  void heal(float amount);
  void harm(float amount);
  void kill();
  void revive();
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_TRAITS_HEALTH_HPP */
