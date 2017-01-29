#include <radix/data/shader/ShaderLoader.hpp>

#include <fstream>
#include <iostream>

#include <radix/env/Util.hpp>
#include <radix/env/Environment.hpp>

namespace radix {

static constexpr GLint MaxGlLogSize = 1024 * 1024 * 2; // 2 MiB

std::map<ShaderEntry, Shader, ShaderEntryCompare> ShaderLoader::shaderCache;

Shader& ShaderLoader::getEverythingShader(const std::string &fragpath, const std::string &geopath,
                                          const std::string &vertpath) {
  std::string fpath = Environment::getDataDir() + "/shaders/" + fragpath;
  std::string gpath;
  if (geopath.empty()) {
    gpath = Environment::getDataDir() + "/shaders/" + fragpath;
  } else {
    gpath = Environment::getDataDir() + "/shaders/" + geopath;
  }
  std::string vpath;
  if (vertpath.empty()) {
    vpath = Environment::getDataDir() + "/shaders/" + fragpath;
  } else {
    vpath = Environment::getDataDir() + "/shaders/" + vertpath;
  }

  ShaderEntry entry{vpath, gpath, fpath};
  auto it = shaderCache.find(entry);
  if (it != shaderCache.end()) {
    return it->second;
  }

  GLuint fragShader = loadShader(fpath, Shader::Fragment);
  GLuint geoShader = loadShader(gpath, Shader::Geometry);
  GLuint vertShader = loadShader(vpath, Shader::Vertex);

  GLuint program = glCreateProgram();
  glAttachShader(program, fragShader);
  glAttachShader(program, geoShader);
  glAttachShader(program, vertShader);

  glLinkProgram(program);

  checkErrors(program, fpath);

  Shader shader(program);
  auto inserted = shaderCache.insert(std::pair<ShaderEntry, Shader>
                                       (entry, shader));
  return inserted.first->second;
}

Shader& ShaderLoader::getGeoAndVertShader(const std::string &geopath, const std::string &vertpath) {
  std::string gpath = Environment::getDataDir() + "/shaders/" + geopath;
  std::string vpath;
  if (vertpath.empty()) {
    vpath = Environment::getDataDir() + "/shaders/" + geopath;
  } else {
    vpath = Environment::getDataDir() + "/shaders/" + vertpath;
  }

  ShaderEntry entry{vpath, geopath, std::string("")};
  auto it = shaderCache.find(entry);
  if (it != shaderCache.end()) {
    return it->second;
  }

  GLuint geoShader = loadShader(gpath, Shader::Geometry);
  GLuint vertShader = loadShader(vpath, Shader::Vertex);

  GLuint program = glCreateProgram();
  glAttachShader(program, geoShader);
  glAttachShader(program, vertShader);

  glLinkProgram(program);

  checkErrors(program, gpath);

  Shader shader(program);
  auto inserted = shaderCache.insert(std::pair<ShaderEntry, Shader>
                                       (entry, shader));
  return inserted.first->second;
}

Shader& ShaderLoader::getFragAndVertShader(const std::string &fragpath, const std::string &vertpath) {
  std::string fpath = Environment::getDataDir() + "/shaders/" + fragpath;
  std::string vpath;
  if (vertpath.empty()) {
    vpath = Environment::getDataDir() + "/shaders/diffuse.vert";
  } else {
    vpath = Environment::getDataDir() + "/shaders/" + vertpath;
  }

  ShaderEntry entry{vpath, std::string(""), fpath};
  auto it = shaderCache.find(entry);
  if (it != shaderCache.end()) {
    return it->second;
  }

  // Load the shaders
  GLuint vertShader = loadShader(vpath, Shader::Vertex);
  GLuint fragShader = loadShader(fpath, Shader::Fragment);

  // Create a program and attach both shaders
  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  glLinkProgram(program);

  // Error checking
  checkErrors(program, fpath);

  Shader shader(program);
  auto inserted = shaderCache.insert(std::pair<ShaderEntry, Shader>
                                       (entry, shader));
  // Return reference to newly inserted Shader
  return inserted.first->second;
}

constexpr static GLenum getGlShaderType(const Shader::Type type) {
  switch (type) {
  case Shader::Vertex:
    return GL_VERTEX_SHADER;
  case Shader::Fragment:
    return GL_FRAGMENT_SHADER;
  case Shader::Geometry:
    return GL_GEOMETRY_SHADER;
  }
  return 0;
}

void ShaderLoader::checkErrors(const GLuint program, std::string &path) {
  GLint success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader") << path << ": program linked";
  } else {
    GLint logSize = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > MaxGlLogSize) {
      throw std::runtime_error("GL reported link log size exceeding maximum");
    }
    std::unique_ptr<char[]> log(new char[static_cast<unsigned long>(logSize)]);
    glGetProgramInfoLog(program, logSize, NULL, log.get());
    Util::Log(Error, "ShaderLoader") << path << ": linking failed:\n" << log.get();
  }

  glValidateProgram(program);

  // Error checking
  glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader") << path << ": program validated";
  } else {
    GLint logSize = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > MaxGlLogSize) {
      throw std::runtime_error("GL reported validation log size exceeding maximum");
    }
    std::unique_ptr<char[]> log(new char[static_cast<unsigned long>(logSize)]);
    glGetProgramInfoLog(program, logSize, NULL, log.get());
    Util::Log(Error, "ShaderLoader") << path << ": validation failed:\n" << log.get();
  }
}

unsigned int ShaderLoader::loadShader(const std::string &path, Shader::Type type) {
  std::ifstream file(path);
  if (not file.is_open()) {
    Util::Log(Error, "ShaderLoader") << "Could not find shader file " << path;
  }
  std::string str;
  std::string file_contents;
  while (std::getline(file, str)) {
    file_contents += str;
    file_contents.push_back('\n');
  }
  const char* buffer = file_contents.c_str();

  GLuint shader = glCreateShader(getGlShaderType(type));
  glShaderSource(shader, 1, &buffer, NULL);
  glCompileShader(shader);

  //Error checking
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  const char *shaderType = "<unknown shader type>";
  switch (type) {
  case Shader::Vertex:
    shaderType = "vertex";
    break;
  case Shader::Fragment:
    shaderType = "fragment";
    break;
  case Shader::Geometry:
    shaderType = "geometry";
    break;
  }
  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader") << path << ": " << shaderType << " shader compiled";
  } else {
    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > MaxGlLogSize) {
      throw std::runtime_error("GL reported compilation log size exceeding maximum");
    }
    std::unique_ptr<char[]> log(new char[static_cast<unsigned long>(logSize)]);
    glGetShaderInfoLog(shader, logSize, &logSize, log.get());

    Util::Log(Error, "ShaderLoader") << path << ": " << shaderType << " shader compilation "
      "failed:\n" << log.get();

    glDeleteShader(shader);
  }

  return shader;
}

} /* namespace radix */
