#ifndef GLYPH_HPP
#define GLYPH_HPP

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

#endif /* GLYPH_HPP */
