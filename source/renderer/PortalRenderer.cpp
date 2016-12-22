#include <radix/renderer/PortalRenderer.hpp>
#include <radix/Viewport.hpp>
#include <epoxy/gl.h>
#include <radix/map/model/MeshLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>
#include <radix/screen/text/FontLoader.hpp>

namespace radix {

PortalRenderer::PortalRenderer(World &world, Renderer &renderer, GameWorld &gameWorld) :
  SubRenderer(world, renderer),
  gameWorld(gameWorld) { }

void PortalRenderer::render() {
}

} /* namespace radix */
