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


int file_get_contents(const char *path, char **buffer, size_t *out_filesize) {

	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		*buffer = NULL;
		*out_filesize = 0;
		return 0;
	}
	size_t filesize;
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	*buffer = (char *)malloc(filesize + 1); // no matter what, add \0 at end
	fseek(file, 0L, SEEK_SET);
	fread(*buffer, filesize, 1, file);
	fclose(file);
	(*buffer)[filesize] = 0; // end string
	if (out_filesize)
		*out_filesize = filesize;
	return 1;
}

unsigned int ShaderLoader::loadShader(const std::string &path,
                                      Shader::Type type) {
  // Create GL Shader
  GLuint shader = glCreateShader(getGlShaderType(type));

  char *filecontent;
  size_t filecontent_bytes;
  int ret = file_get_contents(path.c_str(), &filecontent, &filecontent_bytes);
  if (not ret)
	  Util::Log(Error, "ShaderLoader") << "Could not find shader file " << path;

  // Read buffer to OpenGL Driver
  glShaderSource(shader, 1, &filecontent, NULL);
  // Compile Shader
  glCompileShader(shader);

  // Error checking
  checkShader(shader, path, type);

  free(filecontent);

  return shader;
}

} /* namespace radix */
