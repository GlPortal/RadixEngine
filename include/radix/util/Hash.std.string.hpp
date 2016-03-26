#ifndef RADIX_UTIL_HASH_STD_STRING_HPP
#define RADIX_UTIL_HASH_STD_STRING_HPP

#include <string>

#include <radix/util/Hash.hpp>

namespace radix {

constexpr uint32_t Hash32(const std::string &str) {
  return impl::MurmurHash2(str.data(), str.size(), 0);
}

} /* namespace radix */

#endif /* RADIX_UTIL_HASH_STD_STRING_HPP */
