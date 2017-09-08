#ifndef RADIX_UTIL_NULL_REFERENCE_HPP
#define RADIX_UTIL_NULL_REFERENCE_HPP

namespace radix {
namespace util {

void* getNullptr();

/**
 * Shorthand to get a nullptr-targeting reference.
 * @warning This is undefined behaviour, and should only be used in other cases of *unreachable*
 *          undefined behaviour to make the compiler happy.
 */
template<typename T>
T &NullReference = *reinterpret_cast<T*>(getNullptr());

} /* namespace util */
} /* namespace radix */

#endif /* RADIX_UTIL_NULL_REFERENCE_HPP */
