#ifndef RADIX_ENTITY_MANAGER_HPP
#define RADIX_ENTITY_MANAGER_HPP

#include <list>

#include <radix/Entity.hpp>

namespace radix {

class World;

/** \class EntityManager
 * @brief Manager and container of @ref Entity "entities"
 */
class EntityManager : protected std::list<Entity> {
public:
  using std::list<Entity>::front;
  using std::list<Entity>::back;
  using std::list<Entity>::begin;
  using std::list<Entity>::cbegin;
  using std::list<Entity>::end;
  using std::list<Entity>::cend;
  using std::list<Entity>::rbegin;
  using std::list<Entity>::crbegin;
  using std::list<Entity>::rend;
  using std::list<Entity>::crend;
  using std::list<Entity>::empty;
  using std::list<Entity>::size;
  using std::list<Entity>::max_size;

  World &world;
  EntityManager(World&);

  Entity& create();

  /**
   * Gets the reference to the entity with specified ID.
   * @throws std::out_of_range if no entity with this ID is found.
   */
  Entity& getById(EntityId id);

  /**
   * Gets the reference to the entity with specified name.
   * @throws std::out_of_range if no entity with this name is found.
   * @note No name unicity is enforced by the engine itself. The first entity in the entity list
   * matching this name will be returned.
   */
  Entity& getByName(const std::string &name);
};

} /* namespace radix */

#endif /* RADIX_ENTITY_MANAGER_HPP */
