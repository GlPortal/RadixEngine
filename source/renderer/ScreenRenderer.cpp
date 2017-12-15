#include <radix/renderer/ScreenRenderer.hpp>

#include <radix/core/gl/OpenGL.hpp>

#include <radix/Viewport.hpp>
#include <radix/data/model/MeshLoader.hpp>
#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/data/text/FontLoader.hpp>
#include <radix/util/Profiling.hpp>

namespace radix {

ScreenRenderer::ScreenRenderer(World &w, Renderer &ren, GameWorld &gw) :
  SubRenderer(w, ren),
  gameWorld(gw) { }

void ScreenRenderer::render() {
  for (Screen *screen : *gameWorld.getScreens()) {
    renderScreen(screen);
  }
}

void ScreenRenderer::renderScreen(Screen *screen) {
  PROFILER_BLOCK("ScreenRenderer::renderScreen", profiler::colors::LightGreen100);
  glDepthMask(GL_FALSE);

  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  initCamera();

  Matrix4f widget;
  int xAxisViewportCenter = viewportWidth / 2;
  int yAxisViewportCenter = viewportHeight / 2;
  widget.translate(Vector3f(xAxisViewportCenter, yAxisViewportCenter, -18));
  widget.scale(Vector3f(viewportWidth, viewportHeight, 1));

  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  Shader &shader = ShaderLoader::getShader("color.frag");

  shader.bind();
  glUniform4f(shader.uni("color"), screen->color.r, screen->color.g, screen->color.b, screen->color.a);
  renderer.renderMesh(*renderContext, shader, widget, mesh);

  shader.release();

  for (unsigned int i = 0; i < screen->text.size(); i++) {
    screen->text[i].font = "Pacaya";
    Font font = FontLoader::getFont(screen->text[i].font);
    font.size = screen->text[i].size;
    int textWidth = font.getStringLength(screen->text[i].content);
    Vector3f position(0, 0, screen->text[i].z);

    position.y = viewportHeight - screen->text[i].top;
    Text::Align textAlign = screen->text[i].align;
    position.x = getHorizontalPositionByAlignment(textAlign, viewportWidth, textWidth);

    screen->text[i].position = position;

    renderer.renderText(*renderContext, screen->text[i]);
  }

  glDepthMask(GL_TRUE);
}

int ScreenRenderer::getHorizontalPositionByAlignment(Text::Align align, int viewportWidth, int textWidth) {
  int xAxisViewportCenter = viewportWidth / 2;
  if (align == Text::Center) {
    return xAxisViewportCenter - (textWidth / 2);
  } else if (align == Text::Right) {
    return (xAxisViewportCenter + viewportWidth / 4) - (textWidth / 2);
  }

  // Default assumes left alignment
  return (xAxisViewportCenter - viewportWidth / 4) - (textWidth / 2);
}

} /* namespace radix */
