#include <radix/renderer/ScreenRenderer.hpp>
#include <radix/Viewport.hpp>
#include <epoxy/gl.h>
#include <radix/model/MeshLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>
#include <radix/text/FontLoader.hpp>

namespace radix {

ScreenRenderer::ScreenRenderer(World &w, Renderer &ren) :
  world(w), renderer(ren),
  camera(nullptr),
  viewportWidth(0), viewportHeight(0) {
  renderContext = std::make_unique<RenderContext>(ren);
}

void ScreenRenderer::renderScreen(Screen& screen) {
  glDepthMask(GL_FALSE);

  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  initCamera();

  Matrix4f widget;
  widget.translate(Vector3f((viewportWidth/2), (viewportHeight/2), -18));
  widget.scale(Vector3f(viewportWidth, viewportHeight, 1));

  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  Shader &sh = ShaderLoader::getShader("color.frag");

  sh.bind();
  glUniform4f(sh.uni("color"), screen.color.r, screen.color.g, screen.color.b, screen.color.a);
  renderer.renderMesh(*renderContext, sh, widget, mesh);

  sh.release();


  for (unsigned int i = 0; i < screen.text.size(); i++) { // render text
    screen.text[i].font = "Pacaya";
    Font font = FontLoader::getFont(screen.text[i].font);
    font.size = screen.text[i].size;
    int textWidth =  font.getStringLength(screen.text[i].content);
    Vector3f position(0, 0, screen.text[i].z);

    if (screen.text[i].align == Text::Center) {
      position.x = (viewportWidth/2) - (textWidth/2);
      position.y = viewportHeight - screen.text[i].top;
    } else if (screen.text[i].align == Text::Left) {
      position.x = ((viewportWidth/2) - viewportWidth/4) - (textWidth/2);
      position.y = viewportHeight - screen.text[i].top;
    } else if (screen.text[i].align == Text::Right) {
      position.x = ((viewportWidth/2) + viewportWidth/4) - (textWidth/2);
      position.y = viewportHeight - screen.text[i].top;
    }

    screen.text[i].position = position;

    renderer.renderText(*renderContext, screen.text[i]);
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
