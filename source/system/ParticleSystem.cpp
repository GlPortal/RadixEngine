#include <radix/system/ParticleSystem.hpp>
#include <radix/component/Emitter.hpp>

namespace radix {

ParticleSystem::ParticleSystem(World& world) : System(world) {

}

void ParticleSystem::update(TDelta timeDelta) {
  Util::Log(Debug, "ParticleSystem") << "I am being updated!";
  auto it = emitters.begin();
  while (it++ != emitters.end()) {
    (*it)->update(timeDelta);
  }
}

} /* namespace radix */