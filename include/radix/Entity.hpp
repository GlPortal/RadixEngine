#ifndef RADIX_ENTITY_HPP
#define RADIX_ENTITY_HPP

#include <array>
#include <memory>
#include <type_traits>

#include <radix/core/event/Event.hpp>
#include <radix/component/Component.hpp>
#include <radix/env/Util.hpp>

namespace radix {

class EntityManager;

using EntityId = uint32_t;

/** \class Entity
 * @brief ECS entity, Component container
 */
class Entity {
private:
  friend class EntityManager;

  Entity(Entity&) = delete;
  Entity& operator=(Entity&) = delete;
  Entity(Entity&&) = delete;
  Entity& operator=(Entity&&) = delete;

  void addComponent(ComponentTypeId, Component*);
  void removeComponent(ComponentTypeId);

  /**
   * Entity's name. Used to find a specific entity from the @ref World entity list.
   */
  std::string name;

public:
  struct ComponentAddedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/Entity:ComponentAdded";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    Entity &entity;
    Component &component;
    ComponentAddedEvent(Entity &e, Component &c) :
      entity(e), component(c) {}
  };
  struct ComponentRemovedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/Entity:ComponentRemoved";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    Entity &entity;
    Component &component;
    ComponentRemovedEvent(Entity &e, Component &c) :
      entity(e), component(c) {}
  };
  struct NameChangedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/Entity:NameChanged";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    Entity &entity;
    const std::string oldName;
    NameChangedEvent(Entity &e, const std::string &o) :
      entity(e), oldName(o) {}
  };


  EntityManager &manager;
  Entity(EntityManager &manager, EntityId id) :
    manager(manager), id(id) {
  }

  ~Entity() {
    clearComponents();
  }

  std::array<std::unique_ptr<Component>, Component::MaxId> components;

  /**
   * Entity (instance) identifier.
   * @note IDs are stable and may be used as permanent access keys, but consider using @ref getName
   *       when not referring to a particular unique instance but rather any entity that has.
   */
  EntityId id;

  std::string getName() const {
    return name;
  }
  void setName(const std::string&);

  template<typename T, typename... TArgs>
  T& addComponent(TArgs&&... mArgs) {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    if (hasComponent<T>()) {
      Util::Log(Warning) << "Overwriting a " << typeid(T).name() << " component";
    }
    T* result(new T(*this, std::forward<TArgs>(mArgs)...));
    Component::TypeId id = Component::getTypeId<T>();
    addComponent(id, result);
    return *result;
  }

  template<typename T>
  inline bool hasComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    return components[Component::getTypeId<T>()].get() != nullptr;
  }

  template<typename T>
  inline T& getComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    return *reinterpret_cast<T*>(components[Component::getTypeId<T>()].get());
  }

  template<class T>
  void removeComponent() {
    removeComponent(Component::getTypeId<T>());
  }

  void clearComponents();
};

} /* namespace radix */

#endif /* RADIX_ENTITY_HPP */
