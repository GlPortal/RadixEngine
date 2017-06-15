#ifndef RADIX_ENTITY_MANAGER_HPP
#define RADIX_ENTITY_MANAGER_HPP

#include <list>
#include <unordered_map>
#include <utility>
#include <type_traits>

#include <radix/Entity.hpp>

namespace radix {

class World;

/** \class EntityManager
 * @brief Manager and container of @ref Entity "entities"
 */
class EntityManager final : protected std::list<std::unique_ptr<Entity>> {
private:
  friend class Entity;
  std::unordered_map<EntityId, Entity&> idMap;
  std::unordered_map<std::string, Entity&> nameMap;
  void changeEntityName(Entity&, const std::string&, const std::string&);

  EntityId m_lastAllocatedId;
  EntityId allocateId() {
    return m_lastAllocatedId++;
  }

  void entityAdded(Entity&);

  using Base = std::list<std::unique_ptr<Entity>>;

public:
  struct EntityCreatedEvent : public Event {
    radix_event_declare("radix/EntityManager:EntityCreated")
    Entity &entity;
    EntityCreatedEvent(Entity &e) : entity(e) {}
  };
  struct EntityRemovedEvent : public Event {
    radix_event_declare("radix/EntityManager:EntityRemoved")
    Entity &entity;
    EntityRemovedEvent(Entity &e) : entity(e) {}
  };

  Entity& front() const { return *Base::front(); }
  Entity& back() const { return *Base::back(); }
  class iterator : public Base::iterator {
  public:
    iterator(Base::iterator &&it) : Base::iterator(std::move(it)) {}
    using value_type = Entity;
    using pointer = value_type*;
    using reference = value_type&;
    reference operator*() const {
      return *Base::iterator::operator*();
    }
  };
  class const_iterator : public Base::const_iterator {
  public:
    const_iterator(Base::const_iterator &&it) : Base::const_iterator(std::move(it)) {}
    using value_type = const Entity;
    using pointer = value_type*;
    using reference = value_type&;
    reference operator*() const {
      return *Base::const_iterator::operator*();
    }
  };
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator =	std::reverse_iterator<const_iterator>;
  iterator begin() noexcept { return iterator(Base::begin()); }
  const_iterator begin() const noexcept { return const_iterator(Base::cbegin()); }
  const_iterator cbegin() const noexcept { return const_iterator(Base::cbegin()); }
  iterator end() noexcept { return iterator(Base::end()); }
  const_iterator end() const noexcept { return const_iterator(Base::cend()); }
  const_iterator cend() const noexcept { return const_iterator(Base::cend()); }
  reverse_iterator rbegin() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cbegin()); }
  const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cbegin()); }
  reverse_iterator rend() noexcept { return reverse_iterator(end()); }
  const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cend()); }
  const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cend()); }
  using Base::empty;
  using Base::size;
  using Base::max_size;

  World &world;
  EntityManager(World&);

  template<typename T, typename... Args>
  T& create(Args... args) {
    static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity");
    T *ent = new T(Entity::CreationParams(world, allocateId()), std::forward<Args>(args)...);
    emplace_back(ent);
    entityAdded(*ent);
    return *ent;
  }

  /**
   * Gets the reference to the entity with specified ID.
   * @throws std::out_of_range if no entity with this ID is found.
   */
  Entity& getById(EntityId id);

  /**
   * Gets the reference to the entity with specified name.
   * @throws std::out_of_range if no entity with this name is found.
   */
  Entity& getByName(const std::string &name);
};

} /* namespace radix */

#endif /* RADIX_ENTITY_MANAGER_HPP */
