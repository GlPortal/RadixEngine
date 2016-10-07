#include <radix/renderer/ScreenRenderer.hpp>
#include <radix/Viewport.hpp>
#include <epoxy/gl.h>
#include <radix/model/MeshLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>

namespace radix {

ScreenRenderer::ScreenRenderer(World &w, Renderer &ren) :
  world(w), renderer(ren), camera(nullptr), viewportWidth(0), viewportHeight(0) {
  renderContext = std::make_unique<RenderContext>(ren);
}

void ScreenRenderer::renderScreen(std::shared_ptr<Screen> screen) {
  glDepthMask(GL_FALSE);

  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  initCamera();

  Matrix4f widget;
  widget.translate(Vector3f((viewportWidth/2), (viewportHeight/2), -18));
  widget.scale(Vector3f(viewportWidth, viewportHeight, 1));

  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  Shader &sh = ShaderLoader::getShader("color.frag");
  Vector4f bg(1,1,1,0.2f);

  sh.bind();
  glUniform4f(sh.uni("color"), bg.r, bg.g, bg.b, bg.a);
  renderer.renderMesh(*renderContext, sh, widget, mesh);

  renderer.setFont("Pacaya", 2.5f);
  renderer.setFontColor(Vector4f(1, 1, 1, 1));

  for (unsigned int i = 0; i < screen->text.size(); i++){ // render text
    renderer.setFontSize(screen->text[i].size);
    Vector3f position(0, 0, screen->text[i].z);

    if (screen->text[i].align == "centered"){
      position.x = (viewportWidth/2) - (renderer.getTextWidth(screen->text[i].text)/2);
      position.y = viewportHeight - screen->text[i].top;
    } else if (screen->text[i].align == "left"){
      position.x = ((viewportWidth/2) - viewportWidth/4) - (renderer.getTextWidth(screen->text[i].text)/2);
      position.y = viewportHeight - screen->text[i].top;
    } else if (screen->text[i].align == "right"){
      position.x = ((viewportWidth/2) + viewportWidth/4) - (renderer.getTextWidth(screen->text[i].text)/2);
      position.y = viewportHeight - screen->text[i].top;
    }

    renderer.renderText(*renderContext, screen->text[i].text, position);
  }

  glDepthMask(GL_TRUE);
}

void ScreenRenderer::initCamera() {
  camera = std::make_unique<Camera>();
  camera->setOrthographic();
  camera->setBounds(0, viewportWidth, 0, viewportHeight);
  renderContext->pushCamera(*camera.get());
}

} /* namespace radix */
