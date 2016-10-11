#ifndef RADIX_SCREENRENDERER_HPP
#define RADIX_SCREENRENDERER_HPP

#include <memory>
#include <radix/World.hpp>
#include <radix/Camera.hpp>
#include <radix/screen/Screen.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/renderer/RenderContext.hpp>

namespace radix {

class ScreenRenderer {
public:
  ScreenRenderer(World& w, Renderer& ren);

  void renderScreen(Screen& screen);

  void initCamera();
private:
  World& world;
  Renderer& renderer;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<RenderContext> renderContext;
  int viewportWidth, viewportHeight;
};
} /* namespace radix */

#endif /* RADIX_SCREENRENDERER_HPP */
