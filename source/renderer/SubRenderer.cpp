#include <radix/renderer/SubRenderer.hpp>

#include <memory>

#include <radix/renderer/RenderContext.hpp>

namespace radix {

SubRenderer::SubRenderer(World &w, Renderer &ren) :
    world(w), renderer(ren),
    camera(nullptr),
    viewportWidth(0), viewportHeight(0) {
    renderContext = std::make_unique<RenderContext>(ren);
}

void SubRenderer::initCamera() {
  camera = std::make_unique<Camera>();
  camera->setOrthographic();
  camera->setBounds(0, viewportWidth, 0, viewportHeight);
  renderContext->pushCamera(*camera.get());
}

} /* namespace radix */