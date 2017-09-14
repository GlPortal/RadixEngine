#ifndef RADIX_MATERIAL_LOADER_HPP
#define RADIX_MATERIAL_LOADER_HPP

#include <string>
#include <map>

#include <radix/data/material/Material.hpp>

namespace tinyxml2 {
class XMLHandle;
}

namespace radix {

class MaterialLoader {
  public:
  /**
  * @brief create material from xml file
  *
  * @param path xml file path
  *
  * @return material object
  */
  static const Material loadFromXML(const std::string &path);

  /**
  * @brief get Material from XML file
  *
  * @param name XML file name
  *
  * @return Material Instance
  */
  static const Material &getMaterial(const std::string &name);

  /**
  * @brief get Material from Texture
  *
  * @param name texture file name
  *
  * @return Material Instance
  */
  static const Material &fromTexture(const std::string &name);

  private:
  /**< Cached map from created material*/
  static std::map<std::string, Material> materialCache;
};

} /* namespace radix */

#endif /* RADIX_MATERIAL_LOADER_HPP */
