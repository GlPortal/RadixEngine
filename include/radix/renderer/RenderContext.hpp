#ifndef RADIX_RENDER_CONTEXT_HPP
#define RADIX_RENDER_CONTEXT_HPP

#include <stack>

#include <radix/core/math/Matrix4f.hpp>
#include <radix/Camera.hpp>

namespace radix {

class Renderer;

struct RenderContext {
  Renderer &renderer;
  RenderContext(Renderer &r) : renderer(r) {}

  std::stack<Matrix4f> projStack;
  inline Matrix4f& getProj() {
    return projStack.top();
  }

  std::stack<Matrix4f> viewStack, invViewStack;
  int viewStackMaxDepth;
  inline Matrix4f& getView() {
    return viewStack.top();
  }
  inline Matrix4f& getInvView() {
    return invViewStack.top();
  }
  inline void pushView(const Matrix4f &m) {
    viewStack.push(m);
    invViewStack.push(inverse(m));
  }
  inline void popView() {
    viewStack.pop();
    invViewStack.pop();
  }

  inline void pushCamera(const Camera &c) {
    projStack.push(Matrix4f::Identity);
    c.getProjMatrix(projStack.top());
    viewStack.push(Matrix4f::Identity);
    c.getViewMatrix(viewStack.top());
    invViewStack.push(Matrix4f::Identity);
    c.getInvViewMatrix(invViewStack.top());
  }
  inline void popCamera() {
    projStack.pop();
    viewStack.pop();
    invViewStack.pop();
  }
};

} /* namespace radix */

#endif /* RADIX_RENDER_CONTEXT_HPP */
