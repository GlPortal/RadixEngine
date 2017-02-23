#include <radix/renderer/PortalRenderer.hpp>
#include <radix/Viewport.hpp>
#include <radix/data/model/MeshLoader.hpp>
#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/data/text/FontLoader.hpp>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <epoxy/gl.h>
#endif

namespace radix {

PortalRenderer::PortalRenderer(World &world, Renderer &renderer, GameWorld &gameWorld) :
  SubRenderer(world, renderer),
  gameWorld(gameWorld) { }

void PortalRenderer::render() {
}

} /* namespace radix */
