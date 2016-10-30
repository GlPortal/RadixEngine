#ifndef RADIX_RENDERER_HPP
#define RADIX_RENDERER_HPP

#include <string>

#include <radix/World.hpp>
#include <radix/material/Material.hpp>
#include <radix/shader/Shader.hpp>
#include <radix/core/math/Matrix4f.hpp>
#include <radix/core/math/Rectangle.hpp>
#include <radix/renderer/RenderContext.hpp>
#include <radix/component/Transform.hpp>
#include <radix/renderer/TextRenderer.hpp>
#include <radix/renderer/SubRenderer.hpp>

namespace radix {

class Entity;
struct Viewport;

/** @class Renderer
 * @brief Main renderer - handles sub-renderers and provides low level render functions.
 *
 * This class is responsible
 * for all rendering. You push
 * all of your sub-renderers
 * into a renderer stack, and those
 * sub-renderers then get invoked
 * by one main render function.
 *
 * As well as that, it also provides
 * low-level render functions to be
 * used by specific renderers.
 */
class Renderer {
public:
  /**
   * @param world The world to render
   */
  Renderer(World& world);

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
   * Cycles through all the sub-renderers and calls their render function
   */
  void render();

  /**
   * Renders a string to the screen using signed-distance field text rendering.
   * @param text The text to render
   */
   void renderText(RenderContext &rc, Text text);


  /**
   * Renders a mesh with the specified material and transform determined by the
   * given modelMatrix.
   * @param rc     RenderContext
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

  /**
   * Add a renderer to the render queue
   */
  void addRenderer(SubRenderer& subRenderer);

  /**
   * Remove a renderer from the render queue
   */
  void removeRenderer(SubRenderer& subRenderer);

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

  static Matrix4f clipProjMat(const Entity &ent, const Matrix4f &view, const Matrix4f &proj);

private:
  struct Support {
    bool uniformBuffers;
  } support;

  uint lightsUBO;

  World &world;
  Viewport *viewport;
  RenderContext rc;
  TextRenderer textRenderer;

  std::vector<SubRenderer*> subRenderers;
  int vpWidth, vpHeight;
};

} /* namespace radix */

#endif /* RADIX_RENDERER_HPP */
