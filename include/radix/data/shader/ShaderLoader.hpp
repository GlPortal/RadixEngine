#ifndef RADIX_SHADERLOADER_HPP
#define RADIX_SHADERLOADER_HPP

#include <map>
#include <string>

#include <radix/data/shader/Shader.hpp>

namespace radix {

class ShaderLoader {
public:
  using ShaderCache = std::map<std::pair<std::string, std::string>, Shader>;
  /**
 * @brief getShader           Create shader and cache shader
 * @param fragmentShaderPath  fragment shader path
 * @param vertexShaderPath    vertex shader path [default: diffuse shader]
 * @return                    cached shader
 */
  static Shader &getShader(const std::string &fragmentShaderPath,
                           const std::string &vertexShaderPath = "");
  /**
   * @brief loadShader create GL shader handler.
   * @param path       path to shader source file
   * @param type       GL shader type
   * @return           GL shader handler
   */
  static unsigned int loadShader(const std::string &path, Shader::Type type);
  /**
   * @brief createProgram create GL program handler
   * @param vpath         path to vertex shader file
   * @param fpath         path to fragment shader file
   * @return              GL program handler
   */
  static unsigned int createProgram(const std::string &vertexShaderPath,
                                    const std::string &fragmentShaderPath);

private:
  static ShaderCache shaderCache; /**< Map to created Shaders for caching*/
};

} /* namespace radix */

#endif /* RADIX_SHADERLOADER_HPP */
