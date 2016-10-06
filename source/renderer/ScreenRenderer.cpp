#include <radix/renderer/ScreenRenderer.hpp>
#include <radix/Viewport.hpp>
#include <SDL_opengl.h>

namespace radix {

ScreenRenderer::ScreenRenderer(World &w, Renderer &ren) :
  world(w), renderer(ren), camera(nullptr), viewportWidth(0), viewportHeight(0) {
  renderContext = std::make_unique<RenderContext>(ren);
}

void ScreenRenderer::renderScreen(std::shared_ptr<Screen> screen) {
  glDepthMask(GL_FALSE);

  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  initCamera();

  renderer.setFont("Pacaya", 2.5f);
  renderer.setFontColor(screen->textColor);

  for (unsigned int i = 0; i < screen->text.size(); i++){ // render text
    renderer.setFontSize(screen->text[i].size);
    renderer.renderText(*renderContext, screen->text[i].text, screen->text[i].position);
  }
}

void ScreenRenderer::initCamera() {
  camera = std::make_unique<Camera>();
  camera->setOrthographic();
  camera->setBounds(0, viewportWidth, 0, viewportHeight);
  renderContext->pushCamera(*camera.get());
}

} /* namespace radix */
