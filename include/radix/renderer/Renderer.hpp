#ifndef RADIX_RENDERER_HPP
#define RADIX_RENDERER_HPP

#include <string>

#include <radix/World.hpp>
#include <radix/material/Material.hpp>
#include <radix/shader/Shader.hpp>
#include <radix/text/Font.hpp>
#include <radix/core/math/Matrix4f.hpp>
#include <radix/core/math/Vector4f.hpp>
#include <radix/core/math/Rectangle.hpp>
#include <radix/renderer/RenderContext.hpp>
#include <radix/component/Transform.hpp>

namespace radix {

class Camera;
class Entity;
class Portal;
struct Texture;
class Font;
struct Viewport;

/** @class Renderer
 * @brief Low level graphics renderer.
 *
 * This is the low level graphics renderer
 * it is highly encouraged to move all
 * specialized code into sub-renderers.
 */
class Renderer {
public:
  /**
   * @param World The world to render
   */
  Renderer(World&);

  // TODO: possibly remove to make rendering viewport-stateless
  void setViewport(Viewport *vp) {
    viewport = vp;
  }
  Viewport* getViewport() const {
    return viewport;
  }

  void init();

  /**
   * Initializes the lights in the world on the given shader
   */
  void updateLights(Shader& shader);

  /**
   * Main scene rendering method. To be called only once per frame.
   * @param dtime Time delta since last frame, in seconds
   * @param cam The camera from which we look at the scene
   */
  void render(double dtime, const Camera &cam);

  /**
   * Renders a string to the screen using signed-distance field text rendering.
   * The text is rendered in the font that is currently set with setFont().
   * @param cam  The camera from which we look at the text
   * @param text The text to render
   * @param x    The x-coordinate of the top left corner of the text in window coordinates
   * @param y    The y-coordinate of the top left corner of the text in window coordinates
   */
  void renderText(RenderContext &rc, const std::string &text, Vector3f vector);


  /**
   * Renders a mesh with the specified material and transform determined by the
   * given modelMatrix.
   * @param cam    The camera from which we look at the mesh
   * @param shader Shader to use to render the mesh
   * @param mdlMtx The model matrix determining the position, rotation and scale of the mesh
   * @param mesh   The mesh to render
   * @param mat    The material to render the mesh with
   */
  void renderMesh(RenderContext &rc, Shader &shader, Matrix4f &mdlMtx, const Mesh &mesh,
                  const Material *mat = nullptr);

  void renderMesh(RenderContext &rc, Shader &shader, Matrix4f &mdlMtx, const Mesh &mesh,
                  const Material &mat) {
    return renderMesh(rc, shader, mdlMtx, mesh, &mat);
  }


  static Matrix4f getFrameView(const Matrix4f &src, const Matrix4f &in, const Matrix4f &out);
  static Matrix4f getFrameView(const Matrix4f &src, const Transform &in, const Transform &out) {
    Matrix4f inMat;
    inMat.translate(in.getPosition());
    inMat.rotate(in.getOrientation());
    Matrix4f outMat;
    outMat.translate(out.getPosition());
    outMat.rotate(out.getOrientation());
    return getFrameView(src, inMat, outMat);
  }
  static bool clipViewFrame(const RenderContext &rc, const Mesh &frame,
    const Transform &frameTform, RectangleI &scissor);


  /**
   * Sets the font to use for all future text drawing until changed again
   * @param font Name of the font
   * @param size Size of the text drawn with this font
   */
  void setFont(const std::string &font, float size);

  void setFontSize(float size);

  void setFontColor(const Vector4f color);

  /**
   * Meassures the width of text respecting the current font
   * @param text
   * @return Width in pixels
   */
  int getTextWidth(std::string text);
  static Matrix4f clipProjMat(const Entity &ent, const Matrix4f &view, const Matrix4f &proj);

private:
  struct Support {
    bool uniformBuffers;
  } support;
  unsigned int lightsUBO;
  World &world;
  Viewport *viewport;
  int vpWidth, vpHeight;
  double time;
  Vector4f fontColor;
  Font *font;
  RenderContext rc;
};

} /* namespace radix */

#endif /* RADIX_RENDERER_HPP */
