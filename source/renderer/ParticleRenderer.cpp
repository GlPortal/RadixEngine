#include <radix/renderer/ParticleRenderer.hpp>

#include <list>
#include <SDL_opengl.h>

#include <radix/component/Emitter.hpp>
#include <radix/system/ParticleSystem.hpp>

namespace radix {

ParticleRenderer::ParticleRenderer(World &world, Renderer &renderer) : SubRenderer(world, renderer) {

}

void ParticleRenderer::render() {
  std::list<Emitter*>& emitters = world.systems.get<ParticleSystem>().getEmitters();

  auto it = emitters.begin();
  while (++it != emitters.end()) {
    Emitter* emitter = (*it);
    initCamera();
    emitter->setMatrices(renderContext->getProj(), camera->getPosition(), Vector3f(1, 1, 1),
                        Vector3f(1, 1, 1));
    glBindTexture(GL_TEXTURE_2D, emitter->texture.handle);
    emitter->render();
  }
}

} /* namespace radix */