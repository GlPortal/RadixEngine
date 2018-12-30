#ifndef RADIX_RENDER_CONTEXT_HPP
#define RADIX_RENDER_CONTEXT_HPP

#include <vector>

#include <radix/core/math/Matrix4f.hpp>
#include <radix/Camera.hpp>
#include <radix/data/model/Mesh.hpp>

namespace radix {

class Renderer;
class Transform;

struct RenderContext {
  Renderer &renderer;
  RenderContext(Renderer &r) : renderer(r) {}

  std::vector<Matrix4f> projStack;
  bool projDirty;
  inline Matrix4f& getProj() {
    return projStack.back();
  }
  inline const Matrix4f& getProj() const {
    return projStack.back();
  }
  inline void pushProj(const Matrix4f &m) {
    projStack.push_back(m);
    projDirty = true;
  }
  inline void popProj() {
    projStack.pop_back();
    projDirty = true;
  }

  std::vector<Matrix4f> viewStack, invViewStack;
  bool viewDirty;
  size_t viewStackMaxDepth;
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
    viewDirty = true;
  }
  inline void popView() {
    viewStack.pop_back();
    invViewStack.pop_back();
    viewDirty = true;
  }

  inline void pushCamera(const Camera &c) {
    projStack.push_back(Matrix4f::Identity);
    c.getProjMatrix(projStack.back());
    viewStack.push_back(Matrix4f::Identity);
    c.getViewMatrix(viewStack.back());
    invViewStack.push_back(Matrix4f::Identity);
    c.getInvViewMatrix(invViewStack.back());
    projDirty = viewDirty = true;
  }
  inline void popCamera() {
    projStack.pop_back();
    viewStack.pop_back();
    invViewStack.pop_back();
    projDirty = viewDirty = true;
  }

  using ViewFrameInfo = std::pair<const Mesh&, const Transform&>;
  std::vector<ViewFrameInfo> viewFramesStack;
  inline const ViewFrameInfo getViewFrame() const {
    return viewFramesStack.back();
  }
  inline void pushViewFrame(const ViewFrameInfo &frame) {
    viewFramesStack.push_back(frame);
  }
  inline void popViewFrame() {
    viewFramesStack.pop_back();
  }
};

} /* namespace radix */

#endif /* RADIX_RENDER_CONTEXT_HPP */
