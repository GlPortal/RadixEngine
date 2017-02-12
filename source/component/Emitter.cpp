#include <radix/component/Emitter.hpp>
#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/env/Environment.hpp>
#include <radix/data/texture/TextureLoader.hpp>
#include <radix/env/Util.hpp>

namespace radix {

Emitter::Emitter(Entity &entity, const std::string &ptexture,
                      const Vector3f &pgenPosition, const Vector3f &pgenVelocityMin,
                      const Vector3f &pgenVelocityMax, const Vector4f &pgenColor,
                      float pgenLifeTimeMin, float pgenLifeTimeMax,
                      float pgenSize, int ptoGenerate)
  : Component(entity), toGenerate(ptoGenerate), genPosition(pgenPosition),
                      genVelocityMin(pgenVelocityMin),
                      genVelocityMax(pgenVelocityMax), genColor(pgenColor),
                      genLifeTimeMin(pgenLifeTimeMin), genLifeTimeMax(pgenLifeTimeMax),
                      genSize(pgenSize), particleShader(0), renderShader(0){
  const char* feedbackVaryings[6] = {
    "positionOut",
    "velocityOut",
    "colorOut",
    "lifeTimeOut",
    "sizeOut",
    "typeOut"
  };

  GLuint vertexShader;
  GLuint geoShader;
  GLuint fragmentShader;

  vertexShader = ShaderLoader::loadShader(Environment::getDataDir() + "/shaders/particle.vert",
                                          Shader::Vertex);
  geoShader = ShaderLoader::loadShader(Environment::getDataDir() + "/shaders/particle.geom",
                                       Shader::Geometry);
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, geoShader);
  for(int i = 0; i < 6; i++) {
    glTransformFeedbackVaryings(program, 6, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
  }
  glLinkProgram(program);
  
  particleShader = Shader(program);

  vertexShader = ShaderLoader::loadShader(Environment::getDataDir() + "/shaders/renderpart.vert",
                                          Shader::Vertex);
  geoShader = ShaderLoader::loadShader(Environment::getDataDir() + "/shaders/renderpart.geom",
                                       Shader::Geometry);
  fragmentShader = ShaderLoader::loadShader(Environment::getDataDir() + "/shaders/renderpart.frag",
                                       Shader::Fragment);
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, geoShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  renderShader = Shader(program);

  glGenTransformFeedbacks(1, &transformFeedbackBuffer);
  glGenQueries(1, &query);

  glGenBuffers(2, particleBuffer);
  glGenVertexArrays(2, VAO);

  Particle initParticle;
  initParticle.type = 0;

  for (int i = 0; i < 2; i++) {
    glBindVertexArray(VAO[i]);
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLES, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &initParticle);

    for (int j = 0; j < 6; j++) {
      glEnableVertexAttribArray(i);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*) 12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*) 24);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*) 36);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*) 40);
    glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(Particle), (const GLvoid*) 44);
  }

  curReadBuffer = 0;
  numParticles = 1;

  texture = TextureLoader::getTexture(ptexture);
}

void Emitter::update(TDelta delta) {
  particleShader.bind();
  glUniform1f(particleShader.uni("timePassed"), (GLfloat) delta.msec_d());
  glUniform1f(particleShader.uni("genLifeTimeMin"), genLifeTimeMin);
  glUniform1f(particleShader.uni("genLifeTimeMax"), genLifeTimeMax);
  glUniform1f(particleShader.uni("genSize"), genSize);
  glUniform1i(particleShader.uni("genNum"), toGenerate);
  glUniform3fv(particleShader.uni("genPosition"), 1, (GLfloat*) &genPosition);
  glUniform3fv(particleShader.uni("genVelocityMin"), 1, (GLfloat*) &genVelocityMin);
  glUniform3fv(particleShader.uni("genVelocityMax"), 1, (GLfloat*) &genVelocityMax);
  glUniform4fv(particleShader.uni("genColor"), 1, (GLfloat*) &genColor);

  Vector3f randomSeed = Vector3f(Math::randFloatRange(-10.0f, 20.0f),
                                 Math::randFloatRange(-10.0f, 20.0f),
                                 Math::randFloatRange(-10.0f, 20.0f));
  glUniform3fv(particleShader.uni("randomSeed"), 1, (GLfloat*) &randomSeed);

  glEnable(GL_RASTERIZER_DISCARD);
  glEnableVertexAttribArray(1);
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffer[1 - curReadBuffer]);
  glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
  glDrawArrays(GL_POINTS, 0, numParticles);
  glEndTransformFeedback();
  glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
  glGetQueryiv(query, GL_QUERY_RESULT, &numParticles);
  curReadBuffer = 1- curReadBuffer;
  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
  particleShader.release();

  Util::Log(Debug, "Emitter") << "I have been updated!";
}

void Emitter::setMatrices(Matrix4f pmatrixProjection, Vector3f peye, Vector3f pview, Vector3f pupVector) {
  matrixProjection = pmatrixProjection;
  matrixView = Math::lookAt(peye, pview, pupVector);

  pview = pview - peye;
  normalize(pview);
  quad1 = cross(pview, pupVector);
  normalize(quad1);
  quad2 = cross(pview, quad1);
  normalize(quad2);
}

void Emitter::render() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glDepthMask(0);

  glDisable(GL_RASTERIZER_DISCARD);
  renderShader.bind();
  glUniformMatrix4fv(renderShader.uni("uniformMatrices.projection"), 1, GL_FALSE, (GLfloat*) &matrixProjection);
  glUniformMatrix4fv(renderShader.uni("uniformMatrices.view"), 1, GL_FALSE, (GLfloat*) &matrixView);
  glUniform3fv(renderShader.uni("quad1"), 1, (GLfloat*) &quad1);
  glUniform3fv(renderShader.uni("quad2"), 1, (GLfloat*) &quad2);
  glUniform1f(renderShader.uni("sampler"), 0);

  glBindVertexArray(VAO[curReadBuffer]);
  glDisableVertexAttribArray(1);
  glDrawArrays(GL_POINTS, 0, numParticles);

  glDepthMask(1);
  glDisable(GL_BLEND);
  renderShader.release();

  Util::Log(Debug, "Emitter") << "I have been rendered!";
}

void Emitter::serialize(serine::Archiver &) {

}

} /* namespace radix */