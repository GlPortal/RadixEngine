#include <radix/data/shader/ShaderLoader.hpp>

#include <fstream>
#include <iostream>
#include <memory>

#include <epoxy/gl.h>

#include <radix/env/Environment.hpp>
#include <radix/env/Util.hpp>

#include <ciso646>

namespace radix {

static constexpr GLint MaxGlLogSize = 1024 * 1024 * 2; // 2 MiB

std::map<std::pair<std::string, std::string>, Shader> ShaderLoader::shaderCache;

Shader& ShaderLoader::getShader(const std::string &fragpath, const std::string &vertpath) {
  std::string fpath = Environment::getDataDir() + "/shaders/" + fragpath;
  std::string vpath;
  if (vertpath.empty()) {
    vpath = Environment::getDataDir() + "/shaders/diffuse.vert";
  } else {
    vpath = Environment::getDataDir() + "/shaders/" + vertpath;
  }
  auto it = shaderCache.find(std::pair<std::string, std::string>(fragpath, vertpath));
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
  GLint success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader") << fpath << ": program linked";
  } else {
    GLint logSize = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > MaxGlLogSize) {
      throw std::runtime_error("GL reported link log size exceeding maximum");
    }
    std::unique_ptr<char[]> log(new char[static_cast<unsigned long>(logSize)]);
    glGetProgramInfoLog(program, logSize, NULL, log.get());
    Util::Log(Error, "ShaderLoader") << fpath << ": linking failed:\n" << log.get();
  }

  glValidateProgram(program);

  // Error checking
  glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader") << fpath << ": program validated";
  } else {
    GLint logSize = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > MaxGlLogSize) {
      throw std::runtime_error("GL reported validation log size exceeding maximum");
    }
    std::unique_ptr<char[]> log(new char[static_cast<unsigned long>(logSize)]);
    glGetProgramInfoLog(program, logSize, NULL, log.get());
    Util::Log(Error, "ShaderLoader") << fpath << ": validation failed:\n" << log.get();
  }

  Shader shader(program);
  auto inserted = shaderCache.insert(std::pair<std::pair<std::string, std::string>, Shader>(
    std::pair<std::string, std::string>(fragpath, vertpath), shader));
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
