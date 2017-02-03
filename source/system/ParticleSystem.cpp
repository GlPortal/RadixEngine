#include <radix/system/ParticleSystem.hpp>

namespace radix {

ParticleSystem::ParticleSystem(World& world) : System(world) {

}

void ParticleSystem::update(TDelta timeDelta) {

}

void ParticleSystem::addParticle(Particle particle) {
  particles.push_back(particle);
}

} /* namespace radix */