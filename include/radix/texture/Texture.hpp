#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <epoxy/gl.h>

namespace radix {

struct Texture {
  GLuint handle = 0xBAD7E96;
  int width;
  int height;
};

} /* namespace radix */

#endif /* TEXTURE_HPP */
