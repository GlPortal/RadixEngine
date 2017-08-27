#include <radix/physics/PhysicsDebugDraw.hpp>

#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/data/texture/TextureLoader.hpp>
#include <radix/env/Util.hpp>

#include <radix/core/gl/DebugOutput.hpp>

namespace radix {

PhysicsDebugDraw::PhysicsDebugDraw() :
  vao(0) {
  //gl::DebugOutput::enable();
  setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
  vbo = std::make_unique<VBO>();
}

PhysicsDebugDraw::~PhysicsDebugDraw() {
  if (vao != 0) {
    glDeleteVertexArrays(1, &vao);
  }
}

void PhysicsDebugDraw::draw3dText(const btVector3 &location, const char *textString) {
  // TODO
}

int PhysicsDebugDraw::getDebugMode() const {
  return dbgMode;
}

void PhysicsDebugDraw::setDebugMode(int debugMode) {
  dbgMode = debugMode;
}

void PhysicsDebugDraw::reportErrorWarning(const char *warningString) {
  Util::Log(Warning, "PhysDbg") << warningString;
}

void PhysicsDebugDraw::drawLine(const btVector3 &from,
                                const btVector3 &to,
                                const btVector3 &color) {
  points.emplace_back(PtData{from.x(), from.y(), from.z(), color.x(), color.y(), color.z()});
  points.emplace_back(PtData{to.x(), to.y(), to.z(), color.x(), color.y(), color.z()});
}

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to,
  const btVector3& fromColor, const btVector3& toColor) {
  points.emplace_back(PtData{from.x(), from.y(), from.z(), fromColor.x(), fromColor.y(),
        fromColor.z()});
  points.emplace_back(PtData{to.x(), to.y(), to.z(), toColor.x(), toColor.y(), toColor.z()});
}

void PhysicsDebugDraw::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
  btScalar distance, int lifeTime, const btVector3 &color) {
  drawLine(PointOnB, PointOnB+normalOnB, color);
}


void PhysicsDebugDraw::render(RenderContext &rc) {
  if (points.size() == 0) {
    return;
  }

  glDisable(GL_CULL_FACE);
  Shader &sh = ShaderLoader::getShader("unshaded.frag");
  sh.bind();
  glUniformMatrix4fv(sh.uni("projectionMatrix"), 1, false, rc.getProj().toArray());
  glUniformMatrix4fv(sh.uni("viewMatrix"), 1, false, rc.getView().toArray());
  glUniformMatrix4fv(sh.uni("modelMatrix"), 1, false, Matrix4f::Identity.toArray());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, TextureLoader::getEmptyDiffuse().handle);
  glUniform1i(sh.uni("diffuse"), 0);

  if (vbo->getSize() < points.size()*sizeof(PtData)) {
    vbo->setData(points, VBO::Stream | VBO::Draw);
  } else {
    vbo->update(points);
  }

  if (vao == 0) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    vbo->bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(PtData),
        reinterpret_cast<GLvoid*>(offsetof(PtData, x)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(PtData),
        reinterpret_cast<GLvoid*>(offsetof(PtData, r)));
    glEnableVertexAttribArray(4);
  }

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, points.size());
  glBindVertexArray(0);
  points.clear();
  sh.release();
}

} /* namespace radix */
