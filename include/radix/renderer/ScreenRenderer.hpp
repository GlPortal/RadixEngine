#ifndef RADIX_SCREENRENDERER_HPP
#define RADIX_SCREENRENDERER_HPP

#include <memory>
#include <radix/World.hpp>
#include <radix/Camera.hpp>
#include <radix/screen/Screen.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/renderer/RenderContext.hpp>
#include <radix/renderer/SubRenderer.hpp>

namespace radix {

class ScreenRenderer : public SubRenderer{
public:
  ScreenRenderer(World& w, Renderer& ren);

  void render(Screen& screen);
};
} /* namespace radix */

#endif /* RADIX_SCREENRENDERER_HPP */
