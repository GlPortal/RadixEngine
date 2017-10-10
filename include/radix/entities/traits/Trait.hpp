#ifndef RADIX_ENTITIES_TRAIT_HPP
#define RADIX_ENTITIES_TRAIT_HPP

#include <radix/Entity.hpp>

namespace radix {

class EntityManager;

namespace entities {

class Trait : protected virtual Entity {
protected:
  friend EntityManager;
  friend Entity;

  Trait();

  virtual void onRemoveTrait() {}

public:
  inline Entity& entity() {
    return *this;
  }
  inline const Entity& entity() const {
    return *this;
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_TRAIT_HPP */
