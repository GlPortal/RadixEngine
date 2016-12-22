#ifndef RADIX_GLYPH_HPP
#define RADIX_GLYPH_HPP

#include <radix/model/Mesh.hpp>

namespace radix {

class Glyph {
public:
  int x;
  int y;
  int width;
  int height;
  float xOffset;
  float yOffset;
  float advance;

  Mesh mesh;
};

} /* namespace radix */

#endif /* RADIX_GLYPH_HPP */
