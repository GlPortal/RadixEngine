#ifndef RADIX_PHYSICS_DEBUG_DRAW_HPP
#define RADIX_PHYSICS_DEBUG_DRAW_HPP

#include <memory>
#include <vector>

#include <radix/core/gl/OpenGL.hpp>

#include <bullet/LinearMath/btIDebugDraw.h>

#include <radix/core/gl/VBO.hpp>
#include <radix/renderer/RenderContext.hpp>

namespace radix {

class PhysicsDebugDraw : public btIDebugDraw {
private:
  int dbgMode;
  GLuint vao;
  std::unique_ptr<VBO> vbo;
  struct PtData {
    float x, y, z, r, g, b;
  };
  std::vector<PtData> points;

public:
  PhysicsDebugDraw();
  ~PhysicsDebugDraw();

  void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
  void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor,
                const btVector3 &toColor);
  void reportErrorWarning(const char *warningString);
  void draw3dText(const btVector3 &location, const char *textString);
  void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
    btScalar distance, int lifeTime, const btVector3 &color);
  void setDebugMode(int debugMode);
  int getDebugMode() const;

  void render(RenderContext &rc);
};

} /* namespace radix */

#endif /* RADIX_PHYSICS_DEBUG_DRAW_HPP */
