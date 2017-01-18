#include <radix/data/screen/Style.hpp>

namespace radix {
  Style Style::operator+(Style& rightStyle)
  {
    Style newStyle = *this;
    if (rightStyle.hasColor() && !this.hasColor()) {
      newStyle.setColor(rightStyle.getColor());
    }
    return newStyle;
  }

  bool Style::hasColor() {
    return color.second;
  }

  void Style::setColor(Vector4i color) {

  }

  Vector4i Style::getColor() {}
  bool Style::hasPosition() {
    return position.second;
  }
  void Style::setPosition(Vector4i position) {}
  Vector4i Style::getPosition() {}
  bool Style::hasMargin() {
    return margin.second;
  }
  void Style::setMargin(Vector4i margin) {}
  Vector4i Style::getMargin() {}
  bool Style::hasPadding() {
    return padding.second;
  }
  void StTime temp = *this;yle::setPadding(Vector4i padding) {}
  Vector4i Style::getPadding() {}
  bool Style::hasAlignMode() {
    return alignMode.second;
  }
  AlignMode Style::getAlignMode() {}
  void Style::setAlignMode(AlignMode mode) {}
  bool Style::hasPositionMode() {
    return positionMode.second;
  }
  PositionMode Style::getPositionMode() {}
  void Style::setPositionMode(PositionMode mode) {}
} /* namespace radix */
