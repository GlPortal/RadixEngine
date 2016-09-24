#include <radix/renderer/Renderer.hpp>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <epoxy/gl.h>

#include <SDL2/SDL_timer.h>

#include <radix/model/MeshLoader.hpp>
#include <radix/texture/TextureLoader.hpp>
#include <radix/text/FontLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>

#include <radix/World.hpp>
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
#include <radix/component/ViewFrame.hpp>
#include <radix/env/Environment.hpp>

namespace radix {

Renderer::Renderer(World &w) :
  lightsUBO(0),
  world(w),
  viewport(nullptr),
  vpWidth(0), vpHeight(0),
  fontColor(1, 1, 1, 1),
  font(nullptr),
  rc(*this) {
  support.uniformBuffers = epoxy_has_gl_extension("GL_ARB_uniform_buffer_object");
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

  rc.projStack.resize(1);
  camera.getProjMatrix(rc.projStack.back());

  rc.viewStack.resize(1);
  camera.getViewMatrix(rc.viewStack.back());

  rc.invViewStack.resize(1);
  camera.getInvViewMatrix(rc.invViewStack.back());

  rc.viewFramesStack.clear();

  rc.projDirty = rc.viewDirty = true;

  /* Lights */
  // Currently, the way this is handled is horrendous: in UBO mode, it doesn't allow for lighting
  // changes, and in direct mode light data gets sent over and over again.
  Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
  Shader &metal = ShaderLoader::getShader("metal.frag");
  Shader *const shaders[] = { &diffuse, &metal };
  static int numLights = 0;
  for (uint i = 0; i < sizeof(shaders)/sizeof(*shaders); ++i) {
    glUseProgram(shaders[i]->handle);
    if (support.uniformBuffers) {
      glGetError();
      GLuint lightsUBindex = glGetUniformBlockIndex(diffuse.handle, "lightsUB");
      if (glGetError() == GL_INVALID_OPERATION) {
        Util::Log(Debug, "Renderer") << "Uploading lights: bad shader handle";
      } else if (lightsUBindex == GL_INVALID_INDEX) {
        Util::Log(Debug, "Renderer") << "Uniform Block lightsUB not found";
      }
      GLuint lightsUBbinding = 0;
      glUniformBlockBinding(shaders[i]->handle, lightsUBindex, lightsUBbinding);

      if (lightsUBO == 0) {
        GLint lightsUBsize;
        glGetActiveUniformBlockiv(shaders[i]->handle, lightsUBindex,
          GL_UNIFORM_BLOCK_DATA_SIZE, &lightsUBsize);
        struct {
          uint position;
          uint color;
          uint distance;
          uint energy;
          uint specular;
        } lightStructOffsets;
        uint lightStructSize;
        {
          constexpr const GLchar *const names[] = {"lights[0].position", "lights[0].color", "lights[0].distance",
          "lights[0].energy", "lights[0].specular", "lights[1].position"};
          constexpr uint namesCount = sizeof(names)/sizeof(*names);
          GLuint indices[namesCount];
          glGetUniformIndices(shaders[i]->handle, namesCount, names, indices);
          GLint offsets[namesCount];
          glGetActiveUniformsiv(shaders[i]->handle, namesCount, indices, GL_UNIFORM_OFFSET, offsets);
          lightStructOffsets.position = offsets[0];
          lightStructOffsets.color = offsets[1];
          lightStructOffsets.distance = offsets[2];
          lightStructOffsets.energy = offsets[3];
          lightStructOffsets.specular = offsets[4];
          lightStructSize = offsets[5] - offsets[0];
        }
        Util::Log(Debug, "Renderer") << "struct Light {\n"
          "  position : " << lightStructOffsets.position << '\n' <<
          "  distance : " << lightStructOffsets.distance << '\n' <<
          "  color : "    << lightStructOffsets.color << '\n' <<
          "  energy : "   << lightStructOffsets.energy << '\n' <<
          "  specular : " << lightStructOffsets.specular << '\n' <<
          "} : " << lightStructSize;
        std::unique_ptr<uint8_t[]> buf(new uint8_t[lightsUBsize]);
        for (const Entity &e : world.entities) {
          if (not e.hasComponent<LightSource>()) {
            continue;
          }

          LightSource &ls = e.getComponent<LightSource>();
          const Transform &t = e.getComponent<Transform>();
          const Vector3f &tposition = t.getPosition();
          const uint baseAddr = numLights * lightStructSize;
          memcpy(&buf[baseAddr + lightStructOffsets.position],
                &tposition.x, sizeof(tposition.x)*3);
          memcpy(&buf[baseAddr + lightStructOffsets.color],
                &ls.color.r, sizeof(ls.color.r)*3);
          memcpy(&buf[baseAddr + lightStructOffsets.distance],
                &ls.distance, sizeof(ls.distance));
          memcpy(&buf[baseAddr + lightStructOffsets.energy],
                &ls.energy, sizeof(ls.energy));
          memcpy(&buf[baseAddr + lightStructOffsets.specular],
                &ls.specular, sizeof(ls.specular));

          ++numLights;
        }

        glGenBuffers(1, &lightsUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
        glBufferData(GL_UNIFORM_BUFFER, lightsUBsize, buf.get(), GL_DYNAMIC_DRAW);
        Util::Log(Debug, "Renderer") << "Uploading lights: UBO id=" << lightsUBO << " size=" << lightsUBsize;
      }
    } else {
      numLights = 0;
      for (const Entity &e : world.entities) {
        if (not e.hasComponent<LightSource>()) {
          continue;
        }

        LightSource &ls = e.getComponent<LightSource>();
        const Transform &t = e.getComponent<Transform>();
        std::string index = std::to_string(numLights);
        std::string position = "lights[" + index + "].position";
        std::string color    = "lights[" + index + "].color";
        std::string distance = "lights[" + index + "].distance";
        std::string energy   = "lights[" + index + "].energy";
        std::string specular = "lights[" + index + "].specular";
        const Vector3f &tposition = t.getPosition();
        glUniform3f(shaders[i]->uni(position.c_str()), tposition.x, tposition.y, tposition.z);
        glUniform3f(shaders[i]->uni(color.c_str()), ls.color.x, ls.color.y, ls.color.z);
        glUniform1f(shaders[i]->uni(distance.c_str()), ls.distance);
        glUniform1f(shaders[i]->uni(energy.c_str()), ls.energy);
        glUniform1f(shaders[i]->uni(specular.c_str()), ls.specular);

        ++numLights;
      }
    }
    int numLightsLoc = shaders[i]->uni("numLights");
    glUniform1i(numLightsLoc, numLights);
  }

  renderScene(rc);
}

void Renderer::renderViewFrames(RenderContext &rc) {
  GLboolean save_stencil_test;
  glGetBooleanv(GL_STENCIL_TEST, &save_stencil_test);
  GLboolean save_scissor_test;
  glGetBooleanv(GL_SCISSOR_TEST, &save_scissor_test);

  glEnable(GL_STENCIL_TEST);
  glEnable(GL_SCISSOR_TEST);
  for (Entity &e : world.entities) {
    if (e.hasComponent<ViewFrame>()) {
      const Transform &t = e.getComponent<Transform>();
      Matrix4f inMat; t.getModelMtx(inMat);
      const ViewFrame &vf = e.getComponent<ViewFrame>();
      Matrix4f outMat;
      outMat.translate(vf.view.getPosition());
      outMat.rotate(vf.view.getOrientation());
      Matrix4f frameView = getFrameView(rc.getView(), inMat, outMat);
      rc.pushViewFrame(RenderContext::ViewFrameInfo(vf.mesh, t));
      rc.pushView(frameView);
      renderScene(rc);
      rc.popView();
      rc.popViewFrame();
    }
  }
  if (!save_stencil_test) {
    glDisable(GL_STENCIL_TEST);
  }
  if (!save_scissor_test) {
    glDisable(GL_SCISSOR_TEST);
  }

  // Draw portal in the depth buffer so they are not overwritten
  glClear(GL_DEPTH_BUFFER_BIT);

  GLboolean save_color_mask[4];
  GLboolean save_depth_mask;
  glGetBooleanv(GL_COLOR_WRITEMASK, save_color_mask);
  glGetBooleanv(GL_DEPTH_WRITEMASK, &save_depth_mask);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_TRUE);
  Shader shader = ShaderLoader::getShader("whitefill.frag");
  Matrix4f modelMtx;
  for (size_t i = 0; i < rc.viewFramesStack.size(); i++) {
    renderMesh(rc, shader, modelMtx, rc.viewFramesStack[i].first, nullptr);
  }
  glColorMask(save_color_mask[0], save_color_mask[1], save_color_mask[2], save_color_mask[3]);
  glDepthMask(save_depth_mask);
}

void Renderer::renderViewFrameStencil(RenderContext &rc) {
  GLboolean save_color_mask[4];
  GLboolean save_depth_mask;
  glGetBooleanv(GL_COLOR_WRITEMASK, save_color_mask);
  glGetBooleanv(GL_DEPTH_WRITEMASK, &save_depth_mask);

  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilFunc(GL_NEVER, 0, 0xFF);
  glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
  glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF

  rc.pushView(rc.viewStack[0]);
  Shader shader = ShaderLoader::getShader("whitefill.frag");
  Matrix4f modelMtx; rc.viewFramesStack.back().second.getModelMtx(modelMtx);
  renderMesh(rc, shader, modelMtx, rc.viewFramesStack.back().first, nullptr);
  rc.popView();

  for (size_t i = 1; i < rc.viewStack.size() - 1; i++) {  // -1 to ignore last view
    // Increment intersection for current portal
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
    renderMesh(rc, shader, modelMtx, rc.viewFramesStack.back().first, nullptr);

    // Decremental outer portal -> only sub-portal intersection remains
    glStencilFunc(GL_NEVER, 0, 0xFF);
    glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
    rc.pushView(rc.viewStack[i-1]);
    renderMesh(rc, shader, modelMtx, rc.viewFramesStack.back().first, nullptr);
  }

  //glColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_TRUE);  // blue-ish filter if drawing on white or grey
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  /* Fill 1 or more */
  glStencilFunc(GL_LEQUAL, 1, 0xFF);
  glColorMask(save_color_mask[0], save_color_mask[1], save_color_mask[2], save_color_mask[3]);
  glDepthMask(save_depth_mask);
}

void Renderer::renderScene(RenderContext &rc) {
  if (rc.viewFramesStack.size() > rc.viewStackMaxDepth) {
    return;
  }
  RectangleI scissor;
  if (rc.viewFramesStack.size() > 0) {
    const RenderContext::ViewFrameInfo &vfi = rc.getViewFrame();
    // Don't render further if computed clip rect is zero-sized
    if (!clipViewFrame(rc, vfi.first, vfi.second, scissor)) {
      return;
    }
  }

  glClear(GL_DEPTH_BUFFER_BIT);

  renderViewFrames(rc);

  if (rc.viewFramesStack.size() > 0) {
    glScissor(scissor.x, scissor.y, scissor.w, scissor.h);
    renderViewFrameStencil(rc);
  }

  renderEntities(rc);
}

void Renderer::renderEntities(RenderContext &rc) {
  for (Entity &e : world.entities) {
    if (e.hasComponent<MeshDrawable>()) {
      renderEntity(rc, e);
    }
  }
}

void Renderer::renderEntity(RenderContext &rc, const Entity &e) {
  MeshDrawable &drawable = e.getComponent<MeshDrawable>();
  Matrix4f mtx;
  e.getComponent<Transform>().getModelMtx(mtx);

  if (drawable.material.fancyname.compare("Metal tiles .5x") == 0) {
    Shader &metal = ShaderLoader::getShader("metal.frag");
    renderMesh(rc, metal, mtx, drawable.mesh, drawable.material);
  } else {
    Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
    renderMesh(rc, diffuse, mtx, drawable.mesh, drawable.material);
  }
}

void Renderer::renderPlayer(RenderContext &rc) {
  const Transform &t = world.getPlayer().getComponent<Transform>();
  Matrix4f mtx;
  mtx.translate(t.getPosition() + Vector3f(0, -.5f, 0));
  mtx.rotate(t.getOrientation());
  mtx.scale(Vector3f(1.3f, 1.3f, 1.3f));
  const Mesh &dummy = MeshLoader::getMesh("HumanToken.obj");
  const Material &mat = MaterialLoader::fromTexture("HumanToken.png");

  renderMesh(rc, ShaderLoader::getShader("diffuse.frag"), mtx, dummy, mat);
}

void Renderer::renderText(RenderContext &rc, const std::string &text, Vector3f vector) {
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

    renderMesh(rc, shader, mtx, mesh, mat);
    position.x += letter.advance * font->size;
  }
}

void Renderer::renderMesh(RenderContext &rc, Shader &shader, Matrix4f &mdlMtx,
                          const Mesh &mesh, const Material *mat) {
  glUseProgram(shader.handle);

  glUniformMatrix4fv(shader.uni("projectionMatrix"), 1, false, rc.getProj().toArray());
  glUniformMatrix4fv(shader.uni("viewMatrix"), 1, false, rc.getView().toArray());
  glUniformMatrix4fv(shader.uni("invViewMatrix"), 1, false, rc.getInvView().toArray());

  Matrix3f mdlMtx3 = toMatrix3f(mdlMtx);
  Matrix3f i3 = inverse(mdlMtx3);
  Matrix4f modelTrInv4Matrix = toMatrix4f(transpose(i3));
  glUniformMatrix4fv(shader.uni("modelTrInv4Matrix"), 1, false, modelTrInv4Matrix.toArray());

  glUniformMatrix4fv(shader.uni("modelMatrix"), 1, false, mdlMtx.toArray());

  // Per-vertex color multiplier
  glVertexAttrib4f(shader.att("color"), 1, 1, 1, 1);

  if (support.uniformBuffers) {
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0 /* lightsUBbinding, up there */, lightsUBO);
  }

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

Matrix4f Renderer::getFrameView(const Matrix4f &src, const Matrix4f &in, const Matrix4f &out) {
  Matrix4f rotate180; rotate180.rotate(rad(180), 0, 1, 0);
  return src * in * rotate180 * inverse(out);
}

bool Renderer::clipViewFrame(const RenderContext &rc, const Mesh &frame,
  const Transform &frameTform, RectangleI &scissor) {
  int sw, sh;
  rc.renderer.getViewport()->getSize(&sw, &sh);
  scissor.set(0, 0, sw, sh);
  for (unsigned int v = 0; v < rc.viewStack.size() - 1; ++v) { // -1 to ignore last view
    Vector4f p[4];
    RectangleI r;
    bool found_negative_w = false;
    Matrix4f modelMtx; frameTform.applyModelMtx(modelMtx);
    Matrix4f projMtx = rc.getProj();
    projMtx[0] = projMtx[5] / (1.0f*sw/sh);
    for (int pi = 0; pi < 4; pi++) {
      p[pi] = projMtx
        * rc.viewStack[v]
        * modelMtx
        * Vector4f(frame.vertices[pi], 1);
      if (p[pi].w < 0) {
        found_negative_w = true;
      } else {
        p[pi].x /= p[pi].w;
        p[pi].y /= p[pi].w;
      }
    }
    if (found_negative_w) {
      continue;
    }

    Vector4f min_x, max_x, max_y, min_y;
    min_x = max_x = min_y = max_y = p[0];
    for (int i = 0; i < 4; i++) {
      if (p[i].x < min_x.x) min_x = p[i];
      if (p[i].x > max_x.x) max_x = p[i];
      if (p[i].y < min_y.y) min_y = p[i];
      if (p[i].y > max_y.y) max_y = p[i];
    }

    min_x.x = (std::max(-1.f, min_x.x) + 1) / 2 * sw;
    max_x.x = (std::min( 1.f, max_x.x) + 1) / 2 * sw;
    min_y.y = (std::max(-1.f, min_y.y) + 1) / 2 * sh;
    max_y.y = (std::min( 1.f, max_y.y) + 1) / 2 * sh;

    r.x = min_x.x;
    r.y = min_y.y;
    r.w = max_x.x-min_x.x;
    r.h = max_y.y-min_y.y;

    {
      int r_min_x = std::max(r.x, scissor.x);
      int r_max_x = std::min(r.x+r.w, scissor.x+scissor.w);
      scissor.x = r_min_x;
      scissor.w = r_max_x - scissor.x;
      int r_min_y = std::max(r.y, scissor.y);
      int r_max_y = std::min(r.y+r.h, scissor.y+scissor.h);
      scissor.y = r_min_y;
      scissor.h = r_max_y - scissor.y;
    }
    if (scissor.w <= 0 || scissor.h <= 0) {
      return false;
    }
  }

  return true;
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
