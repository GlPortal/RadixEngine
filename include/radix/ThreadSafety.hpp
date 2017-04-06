#ifndef RADIX_THREAD_SAFETY_HPP
#define RADIX_THREAD_SAFETY_HPP

#include <mutex>

#include <radix/env/Util.hpp>

/*
 * THREAD SAFETY UTILITIES
 */

/*
 * Declares a variable of type #type, and of name #name.
 * Along with that, it declares a mutex of name #name##Mutex.
 * This macro should be used to declare variables that must
 * remain thread safe (almost always since this is a multi-threaded engine).
 * It should not be used when declaring variables on the stack, for (hopefully)
 * obvious reasons. It is recommended to use this in conjunction with
 * RADIX_DECLARE_MUTEX_GETTER, to keep code clean.
 */

#define RADIX_DECLARE_WITH_MUTEX(type, name) type name; mutable std::mutex name##Mutex

/*
 * Declares a getter method for variable declared with RADIX_DECLARE_WITH_MUTEX.
 * Direct mutex and variable access should only be used when in const context.
 */

#define RADIX_DECLARE_SAFE_GETTER(type, name)               \
inline type get##name() const {                             \
  return name;                                              \
}                                                           \
inline type safe##Get##name() {                             \
  Util::Log(Debug, "ThreadSafety") << "Trying to acquire lock " << #name; \
  name##Mutex.lock();                                       \
  Util::Log(Debug, "ThreadSafety") << "Acquired lock " << #name; \
  return name;                                              \
}                                                           \
inline std::mutex& get##name##Mutex() const {               \
  return name##Mutex;                                       \
}                                                           \
inline void relinquish##name##Mutex() {                     \
  name##Mutex.unlock();                                     \
}                                                           \


#endif /* RADIX_THREAD_SAFETY_HPP */
