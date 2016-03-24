#ifndef RADIX_COMPONENT_HPP
#define RADIX_COMPONENT_HPP

#include <cstddef>
#include <type_traits>

#include <serine/Serializable.hpp>

namespace radix {

using ComponentTypeId = std::size_t;

ComponentTypeId getNewComponentTypeId();

/*! \cond PRIVATE */
template<typename T> struct _ComponentTypeId { static ComponentTypeId id; };
template<typename T> ComponentTypeId _ComponentTypeId<T>::id { getNewComponentTypeId() };
/*! \endcond */

class Entity;

/** \class Component
 * @brief Base class to create entity components
 */
class Component : public serine::Serializable {
private:
  // Forbid any copy
  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;

protected:
  Entity &entity;

public:
  using TypeId = ComponentTypeId;
  static constexpr TypeId MaxId = 32;

  /** @brief Gets a component's type ID
   * @param T Component type
   */
  template<typename T> inline static TypeId getTypeId() {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    return _ComponentTypeId<typename std::remove_cv<T>::type>::id;
  }

  inline Component(Entity &ent) noexcept :
    entity(ent) {}

  virtual const char* getName() const = 0;
  virtual TypeId getTypeId() const = 0;

  virtual ~Component() {}
};

inline ComponentTypeId getNewComponentTypeId() {
  static ComponentTypeId lastId(0);
  return lastId++;
}

} /* namespace radix */

#endif /* RADIX_COMPONENT_HPP */
