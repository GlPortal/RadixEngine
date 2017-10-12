#define NOMINMAX

#include <radix/renderer/Renderer.hpp>

#include <algorithm>

#include <radix/core/gl/OpenGL.hpp>

#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/Viewport.hpp>
#include <radix/core/math/Matrix3f.hpp>
#include <radix/entities/traits/LightSourceTrait.hpp>
#include <radix/util/Profiling.hpp>

using namespace radix::entities;

namespace radix {

Renderer::Renderer(World &world) :
  lightsUBO(0),
  world(world),
  viewport(nullptr),
  rc(*this),
  textRenderer(world, *this),
  vpWidth(0), vpHeight(0) {
  support.uniformBuffers = gl::OpenGL::hasExtension("GL_ARB_uniform_buffer_object");
}

void Renderer::init() {
  Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
  updateLights(diffuse);
  Shader &metal = ShaderLoader::getShader("metal.frag");
  updateLights(metal);
}

void Renderer::render() {
  for (SubRenderer *e : subRenderers) {
    e->render();
  }
  glFlush();
  glFinish();
}

void Renderer::updateLights(Shader& shader) {
  PROFILER_BLOCK("Renderer::updateLights");
  shader.bind();

  int numLights = 0;
  for (const Entity &e : world.entityManager) {
    const LightSourceTrait *lstp = dynamic_cast<const LightSourceTrait*>(&e);
    if (lstp == nullptr) {
      continue;
    }

    const LightSourceTrait &ls = *lstp;
    std::string index = std::to_string(numLights);
    std::string position = "lights[" + index + "].position";
    std::string color    = "lights[" + index + "].color";
    std::string distance = "lights[" + index + "].distance";
    std::string energy   = "lights[" + index + "].energy";
    std::string specular = "lights[" + index + "].specular";
    const Vector3f &tposition = e.getPosition();
    glUniform3f(shader.uni(position.c_str()), tposition.x, tposition.y, tposition.z);
    glUniform3f(shader.uni(color.c_str()), ls.color.x, ls.color.y, ls.color.z);
    glUniform1f(shader.uni(distance.c_str()), ls.distance);
    glUniform1f(shader.uni(energy.c_str()), ls.energy);
    glUniform1f(shader.uni(specular.c_str()), ls.specular);

    ++numLights;
  }

  const int numLightsLoc = shader.uni("numLights");
  glUniform1i(numLightsLoc, numLights);

  shader.release();
}

void Renderer::renderText(RenderContext &rc, Text text) {
  textRenderer.renderText(rc, text);
}

void Renderer::renderMesh(RenderContext &rc, Shader &shader, Matrix4f &mdlMtx,
                          const Mesh &mesh, const Material *mat) {
  shader.bind();

  PROFILER_BLOCK("GL pass matrices");
  glUniformMatrix4fv(shader.uni("projectionMatrix"), 1, false, rc.getProj().toArray());
  glUniformMatrix4fv(shader.uni("viewMatrix"), 1, false, rc.getView().toArray());
  glUniformMatrix4fv(shader.uni("invViewMatrix"), 1, false, rc.getInvView().toArray());

  PROFILER_BLOCK("Matrix conversion");
  Matrix3f mdlMtx3 = toMatrix3f(mdlMtx);
  Matrix3f i3 = inverse(mdlMtx3);
  Matrix4f modelTrInv4Matrix = toMatrix4f(transpose(i3));
  PROFILER_END_BLOCK;
  glUniformMatrix4fv(shader.uni("modelTrInv4Matrix"), 1, false, modelTrInv4Matrix.toArray());

  glUniformMatrix4fv(shader.uni("modelMatrix"), 1, false, mdlMtx.toArray());
  PROFILER_END_BLOCK;

  // Per-vertex color multiplier
  if (shader.att("color") > -1) {
    glVertexAttrib4f(shader.att("color"), 1, 1, 1, 1);
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

    r.x = (int) min_x.x;
    r.y = (int) min_y.y;
    r.w = (int) max_x.x-min_x.x;
    r.h = (int) max_y.y-min_y.y;

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

void Renderer::addRenderer(SubRenderer &subRenderer) {
  subRenderers.push_back(&subRenderer);
}

void Renderer::removeRenderer(SubRenderer &subRenderer) {
  subRenderers.erase(std::remove(subRenderers.begin(), subRenderers.end(), &subRenderer), subRenderers.end());
}

} /* namespace radix */
