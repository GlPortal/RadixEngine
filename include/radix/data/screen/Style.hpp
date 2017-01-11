#ifndef RADIX_STYLE_HPP
#define RADIX_STYLE_HPP

#include <radix/core/math/Vector4f.hpp>
#include <radix/core/math/Vector4i.hpp>

namespace radix {
  enum PositionMode {
    absolute,
    relative
  };

  enum AlignMode {
    left,
    right,
    center
  };

  /**
   * Represents element style
   */
  struct Style {
    Vector4f color;
    Vector4i position;
    Vector4i margin;
    Vector4i padding;
    AlignMode alignMode;
    PositionMode positionMode;
  };
} /* namespace radix */

#endif /* RADIX_STYLE_HPP */
