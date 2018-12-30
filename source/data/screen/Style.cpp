#include <radix/data/screen/Style.hpp>
#include <ciso646>

namespace radix {

  Style::Style():
    color(std::make_pair(Vector4i(0), false)),
    position(std::make_pair(Vector4i(0), false)),
    margin(std::make_pair(Vector4i(0), false)),
    padding(std::make_pair(Vector4i(0), false)),
    alignMode(std::make_pair(AlignMode::left, false)),
    positionMode(std::make_pair(PositionMode::relative, false)){
  }

  Style Style::operator+(Style& rightStyle)
  {
    Style newStyle = *this;
    if (rightStyle.hasColor()) {
      newStyle.setColor(rightStyle.getColor());
    }
    if (rightStyle.hasPosition()) {
      newStyle.setPosition(rightStyle.getPosition());
    }
    if (rightStyle.hasMargin()) {
      newStyle.setMargin(rightStyle.getMargin());
    }
    if (rightStyle.hasPadding()) {
      newStyle.setPadding(rightStyle.getPadding());
    }
    if (rightStyle.hasAlignMode()) {
      newStyle.setAlignMode(rightStyle.getAlignMode());
    }
    if (rightStyle.hasPositionMode()) {
      newStyle.setPositionMode(rightStyle.getPositionMode());
    }

    return newStyle;
  }

  bool Style::hasColor() {
    return color.second;
  }

  void Style::setColor(Vector4i color) {
    //color.first = color;
  }

  Vector4i Style::getColor() {
    return color.first;
  }

  bool Style::hasPosition() {
    return position.second;
  }

  void Style::setPosition(Vector4i position) {
    //position.first = position;
  }

  Vector4i Style::getPosition() {
    return position.first;
  }

  bool Style::hasMargin() {
    return margin.second;
  }
  void Style::setMargin(Vector4i margin) {
    //margin.first = margin;
  }

  Vector4i Style::getMargin() {
    return margin.first;
  }

  bool Style::hasPadding() {
    return padding.second;
  }

  void Style::setPadding(Vector4i padding) {
    //padding.first = padding;
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
