#ifndef RADIX_SCREENRENDERER_HPP
#define RADIX_SCREENRENDERER_HPP

#include <memory>
#include <radix/World.hpp>
#include <radix/Camera.hpp>
#include <radix/screen/Screen.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/renderer/SubRenderer.hpp>
#include <radix/GameWorld.hpp>

namespace radix {

class ScreenRenderer : public SubRenderer{
public:
  ScreenRenderer(World& w, Renderer& ren, GameWorld& gw);

  void render();

private:
  GameWorld &gameWorld;
};
} /* namespace radix */

#endif /* RADIX_SCREENRENDERER_HPP */
