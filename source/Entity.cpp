#include <radix/Entity.hpp>

#include <radix/EntityManager.hpp>
#include <radix/World.hpp>

namespace radix {

void Entity::setName(const std::string &newName) {
  const std::string oldName = name;
  name = newName;
  manager.world.event.dispatch(NameChangedEvent(*this, oldName));
}

void Entity::addComponent(ComponentTypeId id, Component *comp) {
  components[id].reset(comp);
  manager.world.event.dispatch(ComponentAddedEvent(*this, *comp));
}

void Entity::removeComponent(ComponentTypeId id) {
  manager.world.event.dispatch(ComponentRemovedEvent(*this, *components[id]));
  components[id].reset(nullptr);
}

void Entity::clearComponents() {
  for (std::unique_ptr<Component> &p : components) {
    manager.world.event.dispatch(ComponentRemovedEvent(*this, *p));
    p.reset(nullptr);
  }
}

};


