#ifndef ENTITY_HPP
#define ENTITY_HPP

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
  Entity(Entity&) = delete;
  Entity(Entity&&) = delete;

  void addComponent(ComponentTypeId, Component*);
  void removeComponent(ComponentTypeId);

public:
  struct ComponentAddedEvent : public Event {
    static constexpr StaticEventType Type = "radix/Entity/ComponentAdded";
    const EventType getType() const {
      return Type;
    }

    Entity &entity;
    Component &component;
    ComponentAddedEvent(Entity &e, Component &c) :
      entity(e), component(c) {}
  };
  struct ComponentRemovedEvent : public Event {
    static constexpr StaticEventType Type = "radix/Entity/ComponentRemoved";
    const EventType getType() const {
      return Type;
    }

    Entity &entity;
    Component &component;
    ComponentRemovedEvent(Entity &e, Component &c) :
      entity(e), component(c) {}
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
   * @note IDs are **not** stable, so don't store/use them across map saves/loads.
   *       Use @ref name instead.
   */
  EntityId id;

  /**
   * Entity's name. Used to find a specific entity from the @ref World entity list.
   */
  std::string name;

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

#endif /* ENTITY_HPP */
