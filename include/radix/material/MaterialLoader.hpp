#ifndef MATERIALLOADER_HPP
#define MATERIALLOADER_HPP

#include <string>
#include <map>

#include <radix/material/Material.hpp>

namespace radix {

class MaterialLoader {
public:
  static const Material loadFromXML(const std::string &path);
  static const Material& getMaterial(const std::string &name);
  static const Material& fromTexture(const std::string &name);
private:
  static std::map<std::string, Material> materialCache;
};

} /* namespace radix */

#endif /* TEXTURELOADER_HPP */
