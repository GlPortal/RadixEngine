#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <list>

#include <radix/Entity.hpp>

namespace radix {

class Scene;

/** \class EntityManager
 * @brief Manager and container of @ref Entity "entities"
 */
class EntityManager : public std::list<Entity> {
public:
  Scene &scene;
  EntityManager(Scene &scene);

  Entity& create();
};

} /* namespace radix */

#endif /* ENTITY_MANAGER_HPP */
