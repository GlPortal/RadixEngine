#include <radix/Entity.hpp>

#include <utility>

#include <radix/World.hpp>

const static std::string Tag = "Entity";

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
  std::string oldName(std::move(m_name));
  m_name = newName;
  world.entityManager.changeEntityName(*this, oldName, newName);
  world.event.dispatch(NameChangedEvent(*this, oldName));
}

}
