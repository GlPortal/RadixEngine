#ifndef RADIX_SUB_RENDERER_HPP
#define RADIX_SUB_RENDERER_HPP

namespace radix {

class World;
class Renderer;

class SubRenderer {
public:
  SubRenderer(World &w, Renderer& ren);

  virtual void render(...) = 0;
protected:
  void initCamera();

  World &world;
  Renderer &renderer;

  std::unique_ptr<Camera> camera;
  std::unique_ptr<RenderContext> renderContext;

  int viewportWidth, viewportHeight;
};

} /* namespace radix */

#endif /* RADIX_SUB_RENDERER_HPP */
