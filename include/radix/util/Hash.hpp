#ifndef RADIX_UTIL_HASH_HPP
#define RADIX_UTIL_HASH_HPP

#include <cstdint>

namespace radix {

namespace impl {

// MurmurHash2, by Austin Appleby, https://sites.google.com/site/murmurhash/
#if 0
constexpr unsigned int MurmurHash2(const void *key, int len, unsigned int seed) {
  static_assert(sizeof(int) == 4, "int type isn't 4 bytes long");
  const unsigned int m = 0x5bd1e995;
  const int r = 24;
  unsigned int h = seed ^ len;
  const unsigned char *data = (const unsigned char*)key;
  while (len >= 4) {
    unsigned int k = *(unsigned int *)data;
    k *= m;
    k ^= k >> r;
    k *= m;
    h *= m;
    h ^= k;
    data += 4;
    len -= 4;
  }
  switch (len) {
  case 3: h ^= data[2] << 16;
  case 2: h ^= data[1] << 8;
  case 1: h ^= data[0];
          h *= m;
  };
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
}
#endif
constexpr unsigned int MurmurHashNeutral2(const char *key, int len, unsigned int seed) {
  static_assert(sizeof(int) == 4, "int type isn't 4 bytes long");
  const unsigned int m = 0x5bd1e995;
  const int r = 24;
  unsigned int h = seed ^ len;
  const char *data = key;
  while (len >= 4) {
    unsigned int k = data[0];
    k |= data[1] << 8;
    k |= data[2] << 16;
    k |= data[3] << 24;
    k *= m; 
    k ^= k >> r; 
    k *= m;
    h *= m;
    h ^= k;
    data += 4;
    len -= 4;
  }
  switch (len) {
  case 3: h ^= data[2] << 16;
  case 2: h ^= data[1] << 8;
  case 1: h ^= data[0];
          h *= m;
  };
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
} 

}

/*constexpr uint32_t Hash32(const void *key, int len) {
  return impl::MurmurHashNeutral2(key, len, 0);
}*/

constexpr int conststrlen(const char *str) {
  return *str ? 1 + conststrlen(str + 1) : 0;
}

constexpr uint32_t Hash32(const char *str) {
  return impl::MurmurHashNeutral2(str, conststrlen(str), 0);
}

} /* namespace radix */

#endif /* RADIX_UTIL_HASH_HPP */
