#include <radix/shader/ShaderLoader.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <epoxy/gl.h>

#include <radix/env/Environment.hpp>
#include <radix/env/Util.hpp>

namespace {
  const int LOG_SIZE = 1024;
}

namespace radix {

std::map<std::pair<std::string, std::string>, Shader> ShaderLoader::shaderCache = { };

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
  int vertShader = loadShader(vpath, Shader::Vertex);
  int fragShader = loadShader(fpath, Shader::Fragment);

  // Create a program and attach both shaders
  int shader = glCreateProgram();
  glAttachShader(shader, vertShader);
  glAttachShader(shader, fragShader);

  glLinkProgram(shader);

  // Error checking
  GLint success = 0;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader") << fpath << ": program linked";
  } else {
    GLint logSize = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    std::unique_ptr<char> log(new char[logSize]);
    glGetProgramInfoLog(shader, logSize, NULL, log.get());
    Util::Log(Error, "ShaderLoader") << fpath << ": linking failed:\n" << log.get();
  }

  glValidateProgram(shader);

  // Error checking
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &success);
  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader") << fpath << ": progam validated";
  } else {
    GLint logSize = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    std::unique_ptr<char> log(new char[logSize]);
    glGetProgramInfoLog(shader, logSize, NULL, log.get());
    Util::Log(Error, "ShaderLoader") << fpath << ": validation failed:\n" << log.get();
  }

  Shader s;
  s.handle = shader;
  auto inserted = shaderCache.insert(std::pair<std::pair<std::string, std::string>, Shader>(
    std::pair<std::string, std::string>(fragpath, vertpath), s));
  // Return reference to newly inserted Shader
  return inserted.first->second;
}

constexpr static GLint getGlShaderType(const Shader::Type type) {
  switch (type) {
  case Shader::Vertex:
    return GL_VERTEX_SHADER;
  case Shader::Fragment:
    return GL_FRAGMENT_SHADER;
  case Shader::Geometry:
    return GL_GEOMETRY_SHADER;
  default:
    ;
  }
  return 0;
}

int ShaderLoader::loadShader(const std::string &path, Shader::Type type) {
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

  int shader = glCreateShader(getGlShaderType(type));
  glShaderSource(shader, 1, (const char**) &buffer, NULL);
  glCompileShader(shader);

  //Error checking
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == GL_TRUE) {
    if (type == Shader::Vertex) {
      Util::Log(Debug, "ShaderLoader") << path << ": vertex shader compiled";
    }
    if (type == Shader::Fragment) {
      Util::Log(Debug, "ShaderLoader") << path << ": fragment shader compiled";
    }
  } else {
    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

    std::unique_ptr<char> log(new char[logSize]);
    glGetShaderInfoLog(shader, logSize, &logSize, log.get());

    if (type == Shader::Vertex) {
      Util::Log(Error, "ShaderLoader") << path << ": vertex shader compilation failed:\n" <<
        log.get();
    }
    if (type == Shader::Fragment) {
      Util::Log(Error, "ShaderLoader") << path << ": fragment shader compilation failed:\n" <<
        log.get();
    }

    glDeleteShader(shader);
  }

  return shader;
}

} /* namespace radix */
