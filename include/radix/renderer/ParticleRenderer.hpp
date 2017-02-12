#ifndef RADIX_PARTICLE_RENDERER_HPP
#define RADIX_PARTICLE_RENDERER_HPP

#include <radix/renderer/SubRenderer.hpp>

namespace radix {

class ParticleRenderer : public SubRenderer {
public:
  ParticleRenderer(World &world, Renderer &renderer);

  void render();
};

} /* namespace radix */

#endif /* RADIX_PARTICLE_RENDERER_HPP */
