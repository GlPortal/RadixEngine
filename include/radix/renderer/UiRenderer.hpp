#ifndef UIRENDERER_HPP
#define UIRENDERER_HPP

#include <radix/renderer/Renderer.hpp>
#include <radix/shader/Shader.hpp>
#include <radix/Viewport.hpp>

namespace radix {

class Renderer;
class Entity;
class Portal;
class Scene;
class Texture;
class Font;

class UiRenderer {
public:
  static void render(Renderer &renderer);
  static void renderScreen(Renderer &renderer);
  static void renderHand(Renderer &renderer);
};

} /* namespace radix */

#endif /* UIRENDERER_HPP */
