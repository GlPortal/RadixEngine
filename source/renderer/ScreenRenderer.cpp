#include <radix/renderer/ScreenRenderer.hpp>
#include <radix/Viewport.hpp>
#include <SDL_opengl.h>

namespace radix {

ScreenRenderer::ScreenRenderer(World &w, Renderer &ren) : world(w), renderer(ren), camera(nullptr), viewportWidth(0), viewportHeight(0) {
  renderContext = std::make_unique<RenderContext>(ren);
}

void ScreenRenderer::renderScreen(std::shared_ptr<Screen> screen) {
  glDepthMask(GL_FALSE);

  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  initCamera();

  renderer.setFont("Pacaya", 2.5f);
  renderer.setFontColor(Vector4f(1, 1, 1, 1));

  for (unsigned int i = 0; i < screen->text.size(); i++){ // render text
    renderer.setFontSize(screen->text[i].size);
    Vector3f position(0, 0, screen->text[i].z);

    if (screen->text[i].align == "centered"){
      position.x = (viewportWidth/2) - (renderer.getTextWidth(screen->text[i].text)/2);
      position.y = (viewportHeight/2) + screen->text[i].top;
    }

    renderer.renderText(*renderContext, screen->text[i].text, position);
  }
}

void ScreenRenderer::initCamera() {
  camera = std::make_unique<Camera>();
  camera->setOrthographic();
  camera->setBounds(0, viewportWidth, 0, viewportHeight);
  renderContext->pushCamera(*camera.get());
}

} /* namespace radix */