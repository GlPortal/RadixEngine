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
  enum class materialAttrib : int {
    diffuse = 0,
    normal = 1,
    specular = 2,
    surface = 3,
    scale = 4,
    kind = 5,
    tags = 6
  };

  /**
  * @brief convert enum class to string
  *
  * @param _type enumerate type 
  *
  * @return const c string type name
  */
  static constexpr const char *typeToString(const materialAttrib _type);

  /**
  * @brief load attribute from xml memory
  *
  * @param root xml root tree
  * @param dir  xml file root directory
  * @param mat  material object
  *
  * @return status of xml attribute
  */
  template <materialAttrib _type>
  static bool loadAttrib(tinyxml2::XMLHandle *root, const std::string &dir,
                         Material &mat);

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
