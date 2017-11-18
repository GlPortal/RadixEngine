#ifndef RADIX_SCREEN_HPP
#define RADIX_SCREEN_HPP

#include <vector>

#include <radix/World.hpp>
#include <radix/data/text/Text.hpp>
#include <radix/data/screen/Element.hpp>
#include <radix/core/math/Vector4f.hpp>
#include <radix/core/event/EventDispatcher.hpp>

namespace radix {

struct ScreenConstructor;

struct Screen {
  Screen(World &world, const ScreenConstructor &constructor);
  Vector4f color;
  std::vector<Text> text;
  std::vector<Element> elements;
  
  /* closing */
  World &world;
  std::string key;
  bool remove = false;
  EventDispatcher::CallbackHolder keyPressedHolder;
};

struct ScreenConstructor {
  Vector4f color;
  std::vector<Text> text;
  std::vector<Element> elements;
  
  /* closing */
  std::string key;
};

} /* namespace radix */

#endif /* RADIX_SCREEN_HPP */
