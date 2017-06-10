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

  /**
   * @brief checkProgram Check program for specific object parameter.
   * @param program      OpenGL program handler.
   * @param pname        Specifies the object parameter.
   * @param fpath        path for fragment shader.
   * @link https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetProgram.xhtml
   */
  static void checkProgram(unsigned int program, unsigned int pname, const std::string fpath);

  /**
   * @brief checkShader Check shader for specific object parameter.
   * @param shader      OpenGL shader handler.
   * @param path        Path to shader.
   * @param type        Shader Type.
   * @link https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGetShaderiv.xml
   */
  static void checkShader(unsigned int shader, const std::string &path, Shader::Type type);

  /**
   * @brief getProgramObjectString get program object paramter string.
   * @param pname                  program object paramter.
   * @return                       string of program object.
   */
  static constexpr const char *getProgramObjectString(unsigned int pname);

  /**
   * @brief getGlShaderType get GL shader type from Shader::Type.
   * @param type            Shader type enum.
   * @return                GL shader type
   */
  constexpr static unsigned int getGlShaderType(const Shader::Type type);

  /**
   * @brief GetShaderTypeString get string from Shader::Type
   * @param type                Shader::Type enum
   * @return                    shader type string
   */
  constexpr static const char *getShaderTypeString(const Shader::Type type);
};

} /* namespace radix */

#endif /* RADIX_SHADERLOADER_HPP */
