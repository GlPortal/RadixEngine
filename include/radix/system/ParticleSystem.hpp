#ifndef RADIX_PARTICLE_SYSTEM_HPP
#define RADIX_PARTICLE_SYSTEM_HPP

#include <map>

#include <radix/system/System.hpp>
#include <radix/BaseGame.hpp>

namespace radix {

class Emitter;

class ParticleSystem : public System {
public:
  ParticleSystem(World&);
  ~ParticleSystem();

  const char* getName() const {
    return "ParticleSystem";
  }

  TypeId getTypeId() const {
    return System::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void update(TDelta timeDelta);

  inline void addEmitter(Emitter &emitter) {
    emitters.push_back(&emitter);
  }

  inline std::list<Emitter*>& getEmitters() {
    return emitters;
  }
private:
  std::list<Emitter*> emitters;
};

} /* namespace radix */

#endif /* RADIX_PARTICLE_SYSTEM_HPP */
