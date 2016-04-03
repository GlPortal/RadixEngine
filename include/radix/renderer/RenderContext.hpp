#ifndef RADIX_RENDER_CONTEXT_HPP
#define RADIX_RENDER_CONTEXT_HPP

#include <vector>

#include <radix/core/math/Matrix4f.hpp>
#include <radix/Camera.hpp>

namespace radix {

class Renderer;

struct RenderContext {
  Renderer &renderer;
  RenderContext(Renderer &r) : renderer(r) {}

  std::vector<Matrix4f> projStack;
  inline Matrix4f& getProj() {
    return projStack.back();
  }
  inline const Matrix4f& getProj() const {
    return projStack.back();
  }

  std::vector<Matrix4f> viewStack, invViewStack;
  int viewStackMaxDepth;
  inline Matrix4f& getView() {
    return viewStack.back();
  }
  inline const Matrix4f& getView() const {
    return viewStack.back();
  }
  inline Matrix4f& getInvView() {
    return invViewStack.back();
  }
  inline const Matrix4f& getInvView() const {
    return invViewStack.back();
  }
  inline void pushView(const Matrix4f &m) {
    viewStack.push_back(m);
    invViewStack.push_back(inverse(m));
  }
  inline void popView() {
    viewStack.pop_back();
    invViewStack.pop_back();
  }

  inline void pushCamera(const Camera &c) {
    projStack.push_back(Matrix4f::Identity);
    c.getProjMatrix(projStack.back());
    viewStack.push_back(Matrix4f::Identity);
    c.getViewMatrix(viewStack.back());
    invViewStack.push_back(Matrix4f::Identity);
    c.getInvViewMatrix(invViewStack.back());
  }
  inline void popCamera() {
    projStack.pop_back();
    viewStack.pop_back();
    invViewStack.pop_back();
  }
};

} /* namespace radix */

#endif /* RADIX_RENDER_CONTEXT_HPP */
