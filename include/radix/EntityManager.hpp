#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <list>

#include <radix/Entity.hpp>

namespace radix {

class World;

/** \class EntityManager
 * @brief Manager and container of @ref Entity "entities"
 */
/// @todo Make inheritance private/protected and add public accessors
class EntityManager : public std::list<Entity> {
public:
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

#endif /* ENTITY_MANAGER_HPP */
