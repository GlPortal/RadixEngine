#ifndef RADIX_ENTITIES_TRAIT_HPP
#define RADIX_ENTITIES_TRAIT_HPP

#include <radix/Entity.hpp>

#define radix_trait_declare(TNP, TN) virtual std::string fullTraitClassName() const override { \
      return TNP TN; \
    } \
    virtual std::string traitClassName() const override { \
      return TN; \
    }

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

  /**
   * @brief Get the full virtual class path, without terminating "Trait"
   */
  virtual std::string fullTraitClassName() const = 0;

  /**
   * @brief Get the virtual class name, without terminating "Trait"
   */
  virtual std::string traitClassName() const = 0;
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_TRAIT_HPP */
