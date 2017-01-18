#ifndef RADIX_STYLE_HPP
#define RADIX_STYLE_HPP

#include <map>
#include <utility>

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
  class Style {
  public:
    Style operator+(Style& rightStyle);
    bool hasColor();
    void setColor(Vector4i color);
    Vector4i getColor();
    bool hasPosition();
    void setPosition(Vector4i position);
    Vector4i getPosition();
    bool hasMargin();
    void setMargin(Vector4i margin);
    Vector4i getMargin();
    bool hasPadding();
    void setPadding(Vector4i padding);
    Vector4i getPadding();
    bool hasAlignMode();
    AlignMode getAlignMode();
    void setAlignMode(AlignMode mode);
    bool hasPositionMode();
    PositionMode getPositionMode();
    void setPositionMode(PositionMode mode);
  private:
    std::map<std::string, Vector4i> vector4iMap;
    std::pair <Vector4i, bool> color;
    std::pair <Vector4i, bool> position;
    std::pair <Vector4i, bool> margin;
    std::pair <Vector4i, bool> padding;
    std::pair <AlignMode, bool> alignMode;
    std::pair <PositionMode, bool> positionMode;
  };
} /* namespace radix */

#endif /* RADIX_STYLE_HPP */
