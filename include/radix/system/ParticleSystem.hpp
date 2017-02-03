#ifndef RADIX_PARTICLESYSTEM_HPP
#define RADIX_PARTICLESYSTEM_HPP

#include <radix/system/System.hpp>
#include <radix/BaseGame.hpp>

namespace radix {

struct Particle {
  float size;
  float minLifeTime;
  float maxLifeTime;
  Vector4f color;
  Vector3f minVelocity;
  Vector3f maxVelocity;
  Vector3f position;
};

class ParticleSystem : public System {
private:
  std::list<Particle> particles;
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

  void addParticle(Particle particle);
};

} /* namespace radix */

#endif /* RADIX_PARTICLESYSTEM_HPP */
