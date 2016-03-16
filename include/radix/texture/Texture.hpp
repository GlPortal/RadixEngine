#ifndef RADIX_TEXTURE_HPP
#define RADIX_TEXTURE_HPP

#include <epoxy/gl.h>

namespace radix {

struct Texture {
  GLuint handle = 0xBAD7E96;
  int width;
  int height;
};

} /* namespace radix */

#endif /* RADIX_TEXTURE_HPP */
