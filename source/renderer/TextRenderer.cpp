#include <radix/renderer/TextRenderer.hpp>

#include <radix/core/gl/OpenGL.hpp>

#include <radix/renderer/Renderer.hpp>
#include <radix/data/text/FontLoader.hpp>
#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/data/material/MaterialLoader.hpp>
#include <radix/core/math/Vector2f.hpp>
#include <radix/core/math/Vector4f.hpp>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <epoxy/gl.h>
#endif

namespace radix {

TextRenderer::TextRenderer(World &w, Renderer &ren) :
  world(w),
  renderer(ren) {
  renderContext = std::make_unique<RenderContext>(ren);
}

int TextRenderer::getTextWidth(std::string text, Font font) {
  return font.getStringLength(text);
}

void TextRenderer::renderText(RenderContext &rc, Text text) {
  // FIXME This should be determined by the currently set font
  const Material &mat = MaterialLoader::fromTexture("Pacaya.png");
  Shader &shader = ShaderLoader::getShader("text.frag");
  shader.bind();
  Vector4f color = text.color;
  glUniform4f(shader.uni("color"), color.x, color.y, color.z, color.w);
  Vector2f position(text.position.x, text.position.y);
  Matrix4f mtx;

  const char *array = text.content.c_str();
  for (unsigned int i = 0; i < text.content.length(); i++) {
    char c = array[i];

    Font font = FontLoader::getFont(text.font);
    font.size = text.size;
    const Glyph &letter = font.getGlyph(c);
    const Mesh &mesh = letter.mesh;

    mtx.setIdentity();
    mtx.translate(Vector3f(position.x + letter.xOffset * font.size,
                  position.y + letter.yOffset * font.size,
                  text.position.z));

    mtx.scale(Vector3f(letter.width * font.size,
                      letter.height * font.size, 1));

    renderer.renderMesh(rc, shader, mtx, mesh, mat);
    position.x += letter.advance * font.size;
  }
  shader.release();
}
} /* namespace radix */
