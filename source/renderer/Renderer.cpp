#include <radix/renderer/Renderer.hpp>

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include <epoxy/gl.h>

#include <SDL2/SDL_timer.h>

#include <radix/model/MeshLoader.hpp>
#include <radix/texture/TextureLoader.hpp>
#include <radix/text/FontLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>

#include <radix/scene/Scene.hpp>
#include <radix/model/Mesh.hpp>
#include <radix/texture/Texture.hpp>
#include <radix/text/Font.hpp>
#include <radix/text/Glyph.hpp>
#include <radix/material/MaterialLoader.hpp>

#include <radix/Camera.hpp>
#include <radix/Viewport.hpp>

#include <radix/core/math/Math.hpp>
#include <radix/core/math/Matrix3f.hpp>
#include <radix/core/math/Vector2f.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Vector4f.hpp>

#include <radix/component/Transform.hpp>
#include <radix/component/MeshDrawable.hpp>
#include <radix/component/LightSource.hpp>
#include <radix/env/Environment.hpp>

#include <radix/renderer/UiRenderer.hpp>
#include <radix/renderer/TerminalRenderer.hpp>

namespace radix {

Renderer::Renderer() :
  viewport(nullptr),
  vpWidth(0), vpHeight(0),
  scene(nullptr),
  fontColor(1, 1, 1, 1),
  font(nullptr),
  portalDepth(2) {
}

void Renderer::setViewport(Viewport *vp) {
  viewport = vp;
  viewport->getSize(&vpWidth, &vpHeight);
}

Viewport* Renderer::getViewport() const {
  return viewport;
}

void Renderer::setScene(Scene *scene) {
  this->scene = scene;
}

void Renderer::setFont(const std::string &font, float size) {
  this->font = &FontLoader::getFont(font);
  this->font->size = size;
}

void Renderer::setFontSize(float size) {
  this->font->size = size;
}

void Renderer::setFontColor(const Vector4f color) {
  this->fontColor = color;
}

int Renderer::getTextWidth(std::string text) {
  return this->font->getStringLength(text);
}

void Renderer::render(double dtime, const Camera &cam) {
  time += dtime;
  viewport->getSize(&vpWidth, &vpHeight);

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0, 0, 0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Camera camera = cam;
  camera.setPerspective();
  camera.setAspect((float)vpWidth / vpHeight);

  Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
  glUseProgram(diffuse.handle);

  /* Lights */ {
    int numLights = 0;
    for (const Entity &e : scene->entities) {
      if (not e.hasComponent<LightSource>()) {
        continue;
      }

      LightSource &ls = e.getComponent<LightSource>();
      if (not ls._uploaded) {
        const Transform &t = e.getComponent<Transform>();
        std::string index = std::to_string(numLights);
        std::string position = "lights[" + index + "].position";
        std::string color = "lights[" + index + "].color";
        std::string distance = "lights[" + index + "].distance";
        std::string energy = "lights[" + index + "].energy";
        std::string specular = "lights[" + index + "].specular";
        const Vector3f &tposition = t.getPosition();
        glUniform3f(diffuse.uni(position.c_str()), tposition.x, tposition.y, tposition.z);
        glUniform3f(diffuse.uni(color.c_str()), ls.color.x, ls.color.y, ls.color.z);
        glUniform1f(diffuse.uni(distance.c_str()), ls.distance);
        glUniform1f(diffuse.uni(energy.c_str()), ls.energy);
        glUniform1f(diffuse.uni(specular.c_str()), ls.specular);
        // ls._uploaded = true;
      }

      ++numLights;
    }
    int numLightsLoc = diffuse.uni("numLights");
    glUniform1i(numLightsLoc, numLights);
  }

  Shader &metal = ShaderLoader::getShader("metal.frag");
  glUseProgram(metal.handle);

  /* Lights */ {
    int numLights = 0;
    for (const Entity &e : scene->entities) {
      if (not e.hasComponent<LightSource>()) {
        continue;
      }

      LightSource &ls = e.getComponent<LightSource>();
      if (not ls._uploaded) {
        const Transform &t = e.getComponent<Transform>();
        std::string index = std::to_string(numLights);
        std::string position = "lights[" + index + "].position";
        std::string color = "lights[" + index + "].color";
        std::string distance = "lights[" + index + "].distance";
        std::string energy = "lights[" + index + "].energy";
        std::string specular = "lights[" + index + "].specular";
        const Vector3f &tposition = t.getPosition();
        glUniform3f(metal.uni(position.c_str()), tposition.x, tposition.y, tposition.z);
        glUniform3f(metal.uni(color.c_str()), ls.color.x, ls.color.y, ls.color.z);
        glUniform1f(metal.uni(distance.c_str()), ls.distance);
        glUniform1f(metal.uni(energy.c_str()), ls.energy);
        glUniform1f(metal.uni(specular.c_str()), ls.specular);
        // ls._uploaded = true;
      }

      ++numLights;
    }
    int numLightsLoc = metal.uni("numLights");
    glUniform1i(numLightsLoc, numLights);
  }

#if 0 /// @todo reintroduce portals
  /* Portals, pass 1 */
  for (EntityPair &p : scene->portalPairs) {
    Entity &pEnt1 = *p.first,
           &pEnt2 = *p.second;
    Portal &portal1 = pEnt1.getComponent<Portal>(),
           &portal2 = pEnt2.getComponent<Portal>();

    // Render portals
    renderPortal(camera, pEnt1, pEnt2);
    renderPortal(camera, pEnt2, pEnt1);
    // Depth buffer
    if (portal1.open and portal2.open) {
      Shader &unshaded = ShaderLoader::getShader("unshaded.frag");

      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);

      const Mesh &portalStencil = MeshLoader::getMesh("PortalStencil.obj");

      Matrix4f mtx;
      pEnt1.getComponent<Transform>().getModelMtx(mtx);
      mtx.scale(portal1.getScaleMult());
      renderMesh(camera, unshaded, mtx, portalStencil);

      pEnt2.getComponent<Transform>().getModelMtx(mtx);
      mtx.scale(portal2.getScaleMult());
      renderMesh(camera, unshaded, mtx, portalStencil);

      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
  }
#endif

  renderScene(camera);

#if 0 /// @todo reintroduce portals
  /* Portals, pass 2 */
  glDepthMask(GL_FALSE);
  for (EntityPair &p : scene->portalPairs) {
    Entity &pEnt1 = *p.first,
           &pEnt2 = *p.second;
    Portal &portal1 = pEnt1.getComponent<Portal>(),
           &portal2 = pEnt2.getComponent<Portal>();

    // Draw simplex noise
    if (portal1.open and portal2.open) {
      double dtOpen = scene->world->getTime()-std::max(portal1.openSince, portal2.openSince);
      if (dtOpen < Portal::NOISE_FADE_DELAY) {
        float al = 1-((float)dtOpen/Portal::NOISE_FADE_DELAY)*2;
        renderPortalNoise(camera, pEnt1, al);
        renderPortalNoise(camera, pEnt2, al);
      }
    } else {
      if (portal1.open) {
        renderPortalNoise(camera, pEnt1, 1.f);
      }
      if (portal2.open) {
        renderPortalNoise(camera, pEnt2, 1.f);
      }
    }

    // Draw overlays
    renderPortalOverlay(camera, pEnt1);
    renderPortalOverlay(camera, pEnt2);
  }
  glDepthMask(GL_TRUE);
#endif

  //Draw GUI
  glClear(GL_DEPTH_BUFFER_BIT);
  UiRenderer::render(*this);
  TerminalRenderer::render(*this);
}

void Renderer::renderScene(const Camera &camera) {
  renderEntities(camera);
}

void Renderer::renderEntities(const Camera &cam) {
  for (Entity &e : scene->entities) {
    if (e.hasComponent<MeshDrawable>()) {
      renderEntity(cam, e);
    }
  }
}

void Renderer::renderEntity(const Camera &cam, const Entity &e) {
  MeshDrawable &drawable = e.getComponent<MeshDrawable>();
  Matrix4f mtx;
  e.getComponent<Transform>().getModelMtx(mtx);

  if (drawable.material.fancyname.compare("Metal tiles .5x") == 0) {
    Shader &metal = ShaderLoader::getShader("metal.frag");
    renderMesh(cam, metal, mtx, drawable.mesh, drawable.material);
  } else {
    Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
    renderMesh(cam, diffuse, mtx, drawable.mesh, drawable.material);
  }
}

void Renderer::renderPlayer(const Camera &cam) {
  const Transform &t = scene->player->getComponent<Transform>();
  Matrix4f mtx;
  mtx.translate(t.getPosition() + Vector3f(0, -.5f, 0));
  mtx.rotate(t.getOrientation());
  mtx.scale(Vector3f(1.3f, 1.3f, 1.3f));
  const Mesh &dummy = MeshLoader::getMesh("HumanToken.obj");
  const Material &mat = MaterialLoader::fromTexture("HumanToken.png");

  renderMesh(cam, ShaderLoader::getShader("diffuse.frag"), mtx, dummy, mat);
}

void Renderer::renderText(const Camera &cam, const std::string &text, Vector3f vector) {
  // FIXME This should be determined by the currently set font
  const Material &mat = MaterialLoader::fromTexture("Pacaya.png");
  Shader &shader = ShaderLoader::getShader("text.frag");
  glUniform4f(shader.uni("color"), fontColor.x, fontColor.y, fontColor.z, fontColor.w);
  Vector2f position(vector.x, vector.y);
  Matrix4f mtx;

  const char *array = text.c_str();
  for (unsigned int i = 0; i < text.length(); i++) {
    char c = array[i];

    const Glyph &letter = font->getGlyph(c);
    const Mesh &mesh = letter.mesh;

    mtx.setIdentity();
    mtx.translate(Vector3f(position.x + letter.xOffset * font->size,
                  position.y + letter.yOffset * font->size,
                  vector.z));

    mtx.scale(Vector3f(letter.width * font->size,
                      letter.height * font->size, 1));

    renderMesh(cam, shader, mtx, mesh, mat);
    position.x += letter.advance * font->size;
  }
}


void Renderer::renderMesh(const Camera &cam, Shader &shader, Matrix4f &mdlMtx,
                          const Mesh &mesh, const Material *mat) {
  glUseProgram(shader.handle);

  Matrix4f projMatrix; cam.getProjMatrix(projMatrix);
  glUniformMatrix4fv(shader.uni("projectionMatrix"), 1, false, projMatrix.toArray());
  Matrix4f viewMatrix; cam.getViewMatrix(viewMatrix);
  glUniformMatrix4fv(shader.uni("viewMatrix"), 1, false, viewMatrix.toArray());
  Matrix4f invViewMatrix = inverse(viewMatrix);
  glUniformMatrix4fv(shader.uni("invViewMatrix"), 1, false, invViewMatrix.toArray());

  Matrix3f mdlMtx3 = toMatrix3f(mdlMtx);
  Matrix3f i3 = inverse(mdlMtx3);
  Matrix4f modelTrInv4Matrix = toMatrix4f(transpose(i3));
  glUniformMatrix4fv(shader.uni("modelTrInv4Matrix"), 1, false, modelTrInv4Matrix.toArray());

  glUniformMatrix4fv(shader.uni("modelMatrix"), 1, false, mdlMtx.toArray());

  // Per-vertex color multiplier
  glVertexAttrib4f(4, 1, 1, 1, 1);

  glBindVertexArray(mesh.handle);
  if (mat) {
    int tiling = shader.uni("tiling");
    glUniform2f(tiling, mat->scaleU, mat->scaleV);
    glUniform1i(shader.uni("diffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->diffuse.handle);
    glUniform1i(shader.uni("normalMap"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mat->normal.handle);
    glUniform1i(shader.uni("specularMap"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mat->specular.handle);
    glUniform1f(shader.uni("shininess"), mat->shininess);
  }
  glDrawArrays(GL_TRIANGLES, 0, mesh.numFaces * 3);
  if (mat) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  glBindVertexArray(0);
}

void Renderer::setCameraInPortal(const Camera &cam, Camera &dest,
                                 const Entity &portal, const Entity &otherPortal) {
  Transform &p1T = portal.getComponent<Transform>();
  Matrix4f p1mat;
  p1mat.translate(p1T.getPosition());
  p1mat.rotate(p1T.getOrientation());
  Transform &p2T = otherPortal.getComponent<Transform>();
  Matrix4f p2mat;
  p2mat.translate(p2T.getPosition());
  p2mat.rotate(p2T.getOrientation());
  Matrix4f rotate180; rotate180.rotate(rad(180), 0, 1, 0);
  Matrix4f view; cam.getViewMatrix(view);
  Matrix4f destView = view * p1mat * rotate180 * inverse(p2mat);

  dest.setPerspective();
  dest.setAspect(cam.getAspect());
  dest.setFovy(cam.getFovy());
  dest.setZNear((p1T.getPosition() - cam.getPosition()).length());
  dest.setViewMatrix(destView);
}

/**
 * TODO Documentation required.
 */
/*
Matrix4f Renderer::clipProjMat(const Entity &ent,
                               const Matrix4f &view, const Matrix4f &proj) {
  const Transform &t = ent.getComponent<Transform>();
  Vector4f clipPlane(Math::toDirection(t.rotation),
                     -dot(Math::toDirection(t.rotation),
                          t.position));
  clipPlane = inverse(transpose(view)) * clipPlane;

  if (clipPlane.w > 0.f){
    return proj;
  }

  Vector4f q = inverse(proj) * Vector4f(
    sign(clipPlane.x),
    sign(clipPlane.y),
    1.0f,
    1.0f
  );

  Vector4f c = clipPlane * (2.0f / dot(clipPlane, q));

  Matrix4f newProj = proj;
  newProj[2] = c.x - newProj[3];
  newProj[6] = c.y - newProj[7];
  newProj[10] = c.z - newProj[11];
  newProj[14] = c.w - newProj[15];
  return newProj;
}
*/

} /* namespace radix */
