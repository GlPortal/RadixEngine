#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>

#include <radix/scene/Scene.hpp>
#include <radix/shader/Shader.hpp>
#include <radix/text/Font.hpp>
#include <radix/core/math/Matrix4f.hpp>
#include <radix/core/math/Vector4f.hpp>
#include <radix/renderer/UiRenderer.hpp>

namespace radix {

class Camera;
class Entity;
class Portal;
class Scene;
class Texture;
class Font;
struct Viewport;

class Renderer {
public:
  Renderer();
  void setViewport(Viewport *vp);
  Viewport* getViewport() const;


  /**
   * Sets the current scene for rendering
   */
  void setScene(Scene *scene);

  /**
   * Getss the current scene
   */
  inline Scene* getScene() const {
    return scene;
  }

  /**
   * Main scene rendering method. To be called only once per frame.
   * @param dtime Time delta since last frame, in seconds
   * @param cam The camera from which we look at the scene
   */
  void render(double dtime, const Camera &cam);

  /**
   * Renders the scene with provided camera parameters
   * @param cam The camera from which we look at the scene
   */
  void renderScene(const Camera &cam);

  void renderUI(const Camera &cam);

  /**
   * Renders all the entities in the scene regardless of shading
   * @param cam The camera from which we look at the scene
   */
  void renderEntities(const Camera &cam);

  /**
   * Renders a single entity regardless of shading
   * @param cam The camera from which we look at the entity
   * @param e   The entity to render
   */
  void renderEntity(const Camera &cam, const Entity &e);

  /**
   * Renders the player character using ambient shading
   * @param cam The camera from which we look at the player
   */
  void renderPlayer(const Camera &cam);


  /**
   * Renders a string to the screen using signed-distance field text rendering.
   * The text is rendered in the font that is currently set with setFont().
   * @param cam  The camera from which we look at the text
   * @param text The text to render
   * @param x    The x-coordinate of the top left corner of the text in window coordinates
   * @param y    The y-coordinate of the top left corner of the text in window coordinates
   */
  void renderText(const Camera &cam, const std::string &text, Vector3f vector);


  /**
   * Renders a mesh with the specified material and transform determined by the
   * given modelMatrix.
   * @param cam    The camera from which we look at the mesh
   * @param shader Shader to use to render the mesh
   * @param mdlMtx The model matrix determining the position, rotation and scale of the mesh
   * @param mesh   The mesh to render
   * @param mat    The material to render the mesh with
   */
  void renderMesh(const Camera &cam, Shader &shader, Matrix4f &mdlMtx, const Mesh &mesh,
                  const Material *mat = nullptr);

  void renderMesh(const Camera &cam, Shader &shader, Matrix4f &mdlMtx, const Mesh &mesh,
                  const Material &mat) {
    return renderMesh(cam, shader, mdlMtx, mesh, &mat);
  }

  /**
   * Set the camera in the portal so rendering from that portal is possible
   * @param cam         The camera from which we look at the portal
   * @param dest        The camera to move inside the portal and point in the right direction
   * @param portal      The portal in which to place the camera
   * @param otherPortal The counterpart of the portal
   */
  void setCameraInPortal(const Camera &cam, Camera &dest, const Entity &portal,
                         const Entity &otherPortal);


  /**
   * Sets the font to use for all future text drawing until changed again
   * @param font Name of the font
   * @param size Size of the text drawn with this font
   */
  void setFont(const std::string &font, float size);

  void setFontSize(float size);

  void setFontColor(const Vector4f color);
  
  int getTextWidth(std::string text);
  static Matrix4f clipProjMat(const Entity &ent, const Matrix4f &view, const Matrix4f &proj);

private:
  Viewport *viewport;
  int vpWidth, vpHeight;
  double time;
  Scene *scene;
  Vector4f fontColor;
  Font *font;
  int portalDepth;
};

} /* namespace radix */

#endif /* RENDERER_HPP */
