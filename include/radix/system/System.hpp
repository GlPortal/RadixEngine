#ifndef RADIX_SYSTEM_HPP
#define RADIX_SYSTEM_HPP

#include <type_traits>

#include <radix/core/types/TimeDelta.hpp>

namespace radix {

using SystemTypeId = std::size_t;

SystemTypeId getNewSystemTypeId();

/*! \cond PRIVATE */
template<typename T> struct _SystemTypeId { static SystemTypeId id; };
template<typename T> SystemTypeId _SystemTypeId<T>::id { getNewSystemTypeId() };
/*! \endcond */

class World;

class System {
protected:
  World &world;

public:
  using TypeId = SystemTypeId;

  /** @brief Gets a component's type ID
   * @param T System type
   */
  template<typename T> inline static TypeId getTypeId() {
    static_assert(std::is_base_of<System, T>::value, "T must be a System");
    return _SystemTypeId<typename std::remove_cv<T>::type>::id;
  }

  System(World &w) : world(w) {}

  virtual const char* getName() const = 0;
  virtual TypeId getTypeId() const = 0;
  virtual bool runsBefore(const System&) { return false; }
  virtual bool runsAfter(const System&) { return false; }
  virtual void update(TDelta dtime) = 0;
};

inline SystemTypeId getNewSystemTypeId() {
  static SystemTypeId lastId(0);
  return lastId++;
}

}

#endif /* RADIX_SYSTEM_HPP */
