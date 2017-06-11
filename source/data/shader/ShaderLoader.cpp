#include <radix/data/shader/ShaderLoader.hpp>

#include <fstream>
#include <iostream>
#include <memory>

#include <radix/core/gl/OpenGL.hpp>

#include <radix/env/Environment.hpp>
#include <radix/env/Util.hpp>

namespace radix {

static constexpr GLint MaxGlLogSize = 1024 * 1024 * 2; // 2 MiB

ShaderLoader::ShaderCache ShaderLoader::shaderCache;

constexpr const char *ShaderLoader::getProgramObjectString(GLenum pname) {
  switch (pname) {
  case GL_LINK_STATUS:
    return "Linking";
  case GL_VALIDATE_STATUS:
    return "Validation";
  }
  return "";
}

void ShaderLoader::checkProgram(GLuint program, GLenum pname, const std::string fpath) {
  // Error checking
  GLint success = 0;
  glGetProgramiv(program, pname, &success);
  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader")
        << fpath << ": program " << getProgramObjectString(pname);
  } else {
    GLint logSize = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > MaxGlLogSize) {
      throw std::runtime_error(std::string("GL reported ") +
                               getProgramObjectString(pname) +
                               " log size exceeding maximum");
    }
    std::unique_ptr<char[]> log(new char[static_cast<unsigned long>(logSize)]);
    glGetProgramInfoLog(program, logSize, NULL, log.get());
    Util::Log(Error, "ShaderLoader")
        << fpath << ": " << getProgramObjectString(pname) << " failed:\n"
        << log.get();
  }
}

unsigned int
ShaderLoader::createProgram(const std::string &vertexShaderPath,
                            const std::string &fragmentShaderPath) {
  // Load the shaders
  GLuint vertShader = loadShader(vertexShaderPath, Shader::Vertex);
  GLuint fragShader = loadShader(fragmentShaderPath, Shader::Fragment);

  // Create a program and attach both shaders
  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  glLinkProgram(program);

  // Error checking
  checkProgram(program, GL_LINK_STATUS, fragmentShaderPath);

  glValidateProgram(program);

  // Error checking
  checkProgram(program, GL_VALIDATE_STATUS, fragmentShaderPath);

  return program;
}

Shader &ShaderLoader::getShader(const std::string &fragpath,
                                const std::string &vertpath) {
  auto fpath = Environment::getDataDir() + "/shaders/" + fragpath;
  auto vpath = (vertpath.empty())
                   ? Environment::getDataDir() + "/shaders/diffuse.vert"
                   : Environment::getDataDir() + "/shaders/" + vertpath;

  auto key = std::make_pair(fragpath, vertpath);
  // Check if shader is cached before
  auto it = shaderCache.find(key);
  if (it != shaderCache.end())
    return it->second;

  // Create a program and attach both shaders
  GLuint program = createProgram(vpath, fpath);

  Shader shader(program);
  auto inserted = shaderCache.insert(std::make_pair(key, shader));
  // Return reference to newly inserted Shader
  return inserted.first->second;
}

constexpr GLenum ShaderLoader::getGlShaderType(const Shader::Type type) {
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

constexpr const char *ShaderLoader::getShaderTypeString(const Shader::Type type) {
  switch (type) {
  case Shader::Vertex:
    return "vertex";
  case Shader::Fragment:
    return "fragment";
  case Shader::Geometry:
    return "geometry";
  }
  return "<unknown shader type>";
}

void ShaderLoader::checkShader(unsigned int shader, const std::string &path, Shader::Type type) {
  // Error checking
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  const char *shaderType = getShaderTypeString(type);

  if (success == GL_TRUE) {
    Util::Log(Debug, "ShaderLoader")
        << path << ": " << shaderType << " shader compiled";
  } else {
    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > MaxGlLogSize) {
      throw std::runtime_error(
          "GL reported compilation log size exceeding maximum"
      );
    }
    std::unique_ptr<char[]> log(new char[static_cast<unsigned long>(logSize)]);
    glGetShaderInfoLog(shader, logSize, &logSize, log.get());

    Util::Log(Error, "ShaderLoader")
        << path << ": " << shaderType << " shader compilation "
                                         "failed:\n"
        << log.get();

    glDeleteShader(shader);
  }
}

unsigned int ShaderLoader::loadShader(const std::string &path,
                                      Shader::Type type) {
  // Read file and point to end of file
  std::ifstream file(path, std::ifstream::ate);
  if (not file.is_open()) {
    Util::Log(Error, "ShaderLoader") << "Could not find shader file " << path;
  }
  // Get file size
  auto fileSize = static_cast<std::size_t>(file.tellg());
  // Point back to beginning of the file
  file.seekg(std::ifstream::beg);

  // Create buffer with file size + 1
  std::unique_ptr<char[]> file_contents(new char[fileSize+1]);
  // Get pointer to the buffer
  char *buffer = file_contents.get();
  // Read file to buffer
  file.read(buffer, static_cast<long>(fileSize));
  // Set end of buffer to 0
  file_contents[fileSize] = 0;

  // Create GL Shader
  GLuint shader = glCreateShader(getGlShaderType(type));
  // Read buffer to OpenGL Driver
  glShaderSource(shader, 1, &buffer, NULL);
  // Compile Shader
  glCompileShader(shader);

  // Error checking
  checkShader(shader, path, type);

  return shader;
}

} /* namespace radix */
