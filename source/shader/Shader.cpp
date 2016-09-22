#include <radix/shader/Shader.hpp>

#include <epoxy/gl.h>

namespace radix {

void Shader::bind() {
    glUseProgram(handle);
}

void Shader::release() {
    glUseProgram(0);
}

int Shader::uni(const std::string &name) {
  // Check if the uniform name is already in our map
  auto it = locationMap.find(name);
  if (it != locationMap.end()) {
    return it->second;
  }

  // If its not, get the location and store it
  GLint loc = glGetUniformLocation(handle, name.c_str());
  locationMap.insert({name, loc});

  return loc;
}

int Shader::att(const std::string &name) {
  // Check if the attribute name is already in our map
  auto it = locationMap.find(name);
  if (it != locationMap.end()) {
    return it->second;
  }

  // If its not, get the location and store it
  GLint loc = glGetAttribLocation(handle, name.c_str());
  locationMap.insert({name, loc});

  return loc;
}

} /* namespace radix */
