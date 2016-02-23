#ifndef SHADERLOADER_HPP
#define SHADERLOADER_HPP

#include <map>
#include <string>
#include <utility>

#include <epoxy/gl.h>

#include <radix/shader/Shader.hpp>

namespace radix {

class ShaderLoader {
public:
  static Shader& getShader(const std::string &fragpath, const std::string &vertpath = "");
  static int loadShader(const std::string &path, GLenum type);
private:
  static std::map<std::pair<std::string, std::string>, Shader> shaderCache;
};

} /* namespace radix */

#endif /* SHADERLOADER_HPP */
