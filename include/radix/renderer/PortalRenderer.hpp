#ifndef RADIX_PORTAL_RENDERER_HPP
#define RADIX_PORTAL_RENDERER_HPP

#include <memory>
#include <radix/World.hpp>
#include <radix/Camera.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/renderer/SubRenderer.hpp>
#include <radix/GameWorld.hpp>

namespace radix {

  class PortalRenderer : public SubRenderer{
  public:
    PortalRenderer(World& world, Renderer& renderer, GameWorld& gameWorld);
    void render();
    GameWorld &gameWorld;
  };

} /* namespace radix */

#endif /* RADIX_PORTAL_RENDERER_HPP */
