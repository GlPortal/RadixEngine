#ifndef RADIX_TEXT_RENDERER_HPP
#define RADIX_TEXT_RENDERER_HPP

#include <string>

#include <radix/World.hpp>
#include <radix/text/Font.hpp>
#include <radix/core/math/Matrix4f.hpp>
#include <radix/renderer/RenderContext.hpp>
#include <radix/text/Text.hpp>

namespace radix {

class TextRenderer {
public:
  TextRenderer(World& w, Renderer& ren);
  void render(double dtime);

  /**
   * Renders a string to the screen using signed-distance field text rendering.
   * @param text The text to render
   */
  void renderText(RenderContext &rc, Text text);

  /**
   * Meassures the width of text respecting the font
   * @param text
   * @param font
   * @return Width in pixels
   */
  int getTextWidth(std::string text, Font font);
  static Matrix4f clipProjMat(const Entity &ent, const Matrix4f &view, const Matrix4f &proj);

private:
  World &world;
  Renderer& renderer;
  double time;
  std::unique_ptr<RenderContext> renderContext;
};

} /* namespace radix */

#endif /* RADIX_TEXT_RENDERER_HPP */
