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
    color.first = color;
  }

  Vector4i Style::getColor() {
    return color.first;
  }

  bool Style::hasPosition() {
    return position.second;
  }

  void Style::setPosition(Vector4i position) {
    position.first = position;
  }

  Vector4i Style::getPosition() {
    return position.first;
  }

  bool Style::hasMargin() {
    return margin.second;
  }
  void Style::setMargin(Vector4i margin) {
    margin.first = margin;
  }

  Vector4i Style::getMargin() {
    return margin.first;
  }

  bool Style::hasPadding() {
    return padding.second;
  }

  void Style::setPadding(Vector4i padding) {
    padding.first = padding;
  }

  Vector4i Style::getPadding() {
    return padding.first;
  }

  bool Style::hasAlignMode() {
    return alignMode.second;
  }

  AlignMode Style::getAlignMode() {
    return alignMode.first;
  }

  void Style::setAlignMode(AlignMode mode) {
    alignMode.first = mode;
  }

  bool Style::hasPositionMode() {
    return positionMode.second;
  }

  PositionMode Style::getPositionMode() {
    return positionMode.first;
  }

  void Style::setPositionMode(PositionMode mode) {
    positionMode.first = mode;
  }

} /* namespace radix */
