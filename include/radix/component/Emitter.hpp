#ifndef RADIX_EMITTER_HPP
#define RADIX_EMITTER_HPP

#include <radix/component/Component.hpp>
#include <radix/data/texture/Texture.hpp>
#include <radix/data/shader/Shader.hpp>
#include <radix/core/types/TimeDelta.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Vector4f.hpp>
#include <radix/core/math/Matrix4f.hpp>
#include <radix/data/texture/Texture.hpp>

namespace radix {

#define MAX_PARTICLES 1000000

struct Particle {
  int type;
  float size;
  float lifeTime;
  Vector4f color;
  Vector3f velocity;
  Vector3f position;
};

class Emitter : public Component {
public:
  Emitter(Entity &entity, const std::string &ptexture,
          const Vector3f &pgenPosition, const Vector3f &pgenVelocityMin,
          const Vector3f &pgenVelocityMax, const Vector4f &pgenColor,
          float pgenLifeTimeMin, float pgenLifeTimeMax,
          float pgenSize, int ptoGenerate);

  const char* getName() const {
    return "Emitter";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver&);

  void update(TDelta delta);

  void setMatrices(Matrix4f pmatrixProjection, Vector3f peye, Vector3f pview, Vector3f pupVector);

  void render();

  Texture texture;
private:
  unsigned int transformFeedbackBuffer;
  unsigned int particleBuffer[2];
  unsigned int VAO[2];
  unsigned int query;
  int curReadBuffer;
  int numParticles;
  int toGenerate;
  Vector3f genPosition;
  Vector3f genVelocityMin, genVelocityMax;
  Vector4f genColor;
  Matrix4f matrixProjection;
  Matrix4f matrixView;
  Vector3f quad1, quad2;
  float genLifeTimeMin, genLifeTimeMax;
  float genSize;
  Shader particleShader;
  Shader renderShader;
};

} /* namespace radix */

#endif /* RADIX_EMITTER_HPP */
