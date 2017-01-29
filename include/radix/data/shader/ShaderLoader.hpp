#ifndef RADIX_SHADERLOADER_HPP
#define RADIX_SHADERLOADER_HPP

#include <map>
#include <string>

#include <epoxy/gl.h>

#include <radix/data/shader/Shader.hpp>

namespace radix {

struct ShaderEntry {
  std::string vert;
  std::string geo;
  std::string frag;
};

struct ShaderEntryCompare {
  bool operator()(const ShaderEntry &rhs, const ShaderEntry &lhs) const {
    if (rhs.vert != lhs.vert)
      return rhs.vert < lhs.vert;
    else if (rhs.geo != lhs.geo)
      return rhs.geo < lhs.geo;
    else if (rhs.frag != lhs.frag)
      return rhs.frag < lhs.frag;
    else return false;
  }
};

class ShaderLoader {
public:
  static Shader& getEverythingShader(const std::string &fragpath, const std::string &geopath = "",
                                     const std::string &vertpath = "");
  static Shader& getGeoAndVertShader(const std::string &geopath, const std::string &vertpath = "");
  static Shader& getFragAndVertShader(const std::string &fragpath, const std::string &vertpath = "");
  static unsigned int loadShader(const std::string &path, Shader::Type type);
private:
  static void checkErrors(const GLuint program, std::string &fpath);
  static std::map<ShaderEntry, Shader, ShaderEntryCompare> shaderCache;
};

} /* namespace radix */

#endif /* RADIX_SHADERLOADER_HPP */
