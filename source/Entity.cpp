#include <radix/Entity.hpp>

#include <utility>

#include <radix/entities/traits/Trait.hpp>
#include <radix/World.hpp>

static const char *Tag = "Entity";

namespace radix {

Entity::Entity(const CreationParams &cp) :
  world(cp.world), id(cp.id) {
}
Entity::~Entity() {
}

void Entity::init() {
  Util::Log(Verbose, Tag) << "Create " << className() << '(' << id << ')';
}

void Entity::setPosition(const Vector3f &val) {
  position = val;
}

void Entity::setScale(const Vector3f &val) {
  scale = val;
}

void Entity::setOrientation(const Quaternion &val) {
  orientation = val;
}

void Entity::setName(const std::string &newName) {
  onNameChange(newName);
  std::string oldName(std::move(m_name));
  m_name = newName;
  world.entityManager.changeEntityName(*this, oldName, newName);
  world.event.dispatch(NameChangedEvent(*this, oldName));
}

void Entity::remove() {
  for (auto trit = m_traits.rbegin(); trit != m_traits.rend(); ++trit) {
    (*trit)->onRemoveTrait();
  }
  onRemove();
  world.entityManager.queueDeleteEntity(this);
}

}
