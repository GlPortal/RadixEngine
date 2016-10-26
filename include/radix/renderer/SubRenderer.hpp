#ifndef RADIX_SUB_RENDERER_HPP
#define RADIX_SUB_RENDERER_HPP

namespace radix {

class SubRenderer {
public:
  SubRenderer();

  virtual void render() = 0;
protected:
  World &world;
  Renderer &renderer;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<RenderContext> renderContext;
  int viewportWidth, viewportHeight;
};

} /* namespace radix */

#endif /* RADIX_SUB_RENDERER_HPP */
