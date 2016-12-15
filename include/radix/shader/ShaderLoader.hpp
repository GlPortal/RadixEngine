#ifndef RADIX_SHADERLOADER_HPP
#define RADIX_SHADERLOADER_HPP

#include <map>
#include <string>

#include <radix/shader/Shader.hpp>

namespace radix {

class ShaderLoader {
public:
  static Shader& getShader(const std::string &fragpath, const std::string &vertpath = "");
  static uint loadShader(const std::string &path, Shader::Type type);
private:
  static std::map<std::pair<std::string, std::string>, Shader> shaderCache;
};

} /* namespace radix */

#endif /* RADIX_SHADERLOADER_HPP */
