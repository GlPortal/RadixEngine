#ifndef RADIX_COMPONENT_HEALTH_HPP
#define RADIX_COMPONENT_HEALTH_HPP

#include <radix/component/Component.hpp>

namespace radix {

class Health : public Component {
public:
  float maxHealth;
  float health;

  Health(Entity &ent);

  const char* getName() const {
    return "Health";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver&);

  bool isAlive();
  void heal(float amount);
  void harm(float amount);
  void kill();
  void revive();
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_HEALTH_HPP */
