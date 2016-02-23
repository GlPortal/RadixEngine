#ifndef TERMINAL_RENDERER_HPP
#define TERMINAL_RENDERER_HPP

#include <string>

#include <radix/renderer/Renderer.hpp>
#include <radix/shader/Shader.hpp>

namespace radix {

class Renderer;
class Viewport;
class Entity;
class Portal;
class Scene;
class Texture;
class Font;

class TerminalRenderer {
public:
  static void render(Renderer &renderer);
};

} /* namespace radix */

#endif /* TERMINAL_RENDERER_HPP */
