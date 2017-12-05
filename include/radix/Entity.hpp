#ifndef RADIX_ENTITY_HPP
#define RADIX_ENTITY_HPP

#include <array>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <RadixEntity/Entity.hpp>

#include <radix/Transform.hpp>
#include <radix/core/event/Event.hpp>
#include <radix/core/types/TimeDelta.hpp>
#include <radix/env/Util.hpp>
#include <radix/util/NullReference.hpp>

namespace radix {

class World;

using EntityId = uint32_t;

namespace entities {
class Trait;
}

/** \class Entity
 * It is created like this:
 * @snippet source/data/map/XmlMapLoader.cpp Creating an Entity
 */
class Entity : public entity::Entity, protected Transform {
private:
  friend class EntityManager;

  Entity(Entity&) = delete;
  Entity& operator=(Entity&) = delete;
  Entity(Entity&&) = delete;
  Entity& operator=(Entity&&) = delete;

  void init();

  /**
   * Entity's name. Used to find a specific entity from the @ref World entity list.
   */
  std::string m_name;

protected:
  /**
   * List of traits contained within this entity instance.
   */
  std::vector<entities::Trait*> m_traits;

  Entity() :
    world(util::NullReference<World>) {
    throw std::invalid_argument("You forgot to properly instantiate the Entity base");
  }

public:
  struct NameChangedEvent : public Event {
    radix_event_declare("radix/Entity:NameChanged")

    Entity &entity;
    const std::string oldName;
    NameChangedEvent(Entity &e, const std::string &o) :
      entity(e), oldName(o) {}
  };


  World &world;

  struct CreationParams {
    World &world;
    EntityId id;
    CreationParams(World &world, EntityId id) :
      world(world),
      id(id) {
    }
  };

  Entity(const CreationParams &cp);
  virtual ~Entity();


  /**
   * Entity (instance) identifier.
   * @note IDs are stable and may be used as permanent access keys, but consider using @ref getName
   *       when not referring to a particular unique instance but rather any entity that has.
   */
  EntityId id;

  inline bool operator==(const Entity &o) const {
    return id == o.id;
  }
  inline bool operator!=(const Entity &o) const {
    return id != o.id;
  }


  inline void privSetPosition(const Vector3f &v) {
    position = v;
  }
  inline void privSetScale(const Vector3f &v) {
    scale = v;
  }
  inline void privSetOrientation(const Quaternion &v) {
    orientation = v;
  }

  inline const Vector3f& getPosition() const {
    return position;
  }
  virtual void setPosition(const Vector3f&);

  inline const Vector3f& getScale() const {
    return scale;
  }
  virtual void setScale(const Vector3f&);

  inline const Quaternion& getOrientation() const {
    return orientation;
  }
  virtual void setOrientation(const Quaternion&);
  using Transform::getModelMtx;
  using Transform::applyModelMtx;
  using Transform::operator btTransform;
  inline const Transform& transform() const {
    return *static_cast<const Transform*>(this);
  }

  std::string name() const {
    return m_name;
  }
  void setName(const std::string&);
  virtual void onNameChange(const std::string &newName) {}

  void remove();
  virtual void onRemove() {}

  virtual std::string fullClassName() const = 0;
  virtual std::string className() const = 0;

  virtual void tick(TDelta) {}
};

} /* namespace radix */

#endif /* RADIX_ENTITY_HPP */
