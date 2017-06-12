#ifndef RADIX_TEXTURELOADER_HPP
#define RADIX_TEXTURELOADER_HPP

#include <map>
#include <string>

#include <radix/data/texture/Texture.hpp>

namespace radix {

class TextureLoader {
    using CachedTexture = std::map<std::string, Texture>;
public:
  /**
 * @brief getEmptyTexture get Empty texture
 * @param name            Texture name for caching
 * @param pixel           pointer to image
 * @return                return Empty Texture
 */
  static Texture getEmptyTexture(const std::string &name,
                                 const char *pixel = "\xFF\xFF\xFF");

  /**
   * @brief getEmptyDiffuse get Empty Diffuse Texture
   * @return
   */
  static Texture getEmptyDiffuse();

  /**
   * @brief getEmptyNormal get Empty Normal Texture
   * @return
   */
  static Texture getEmptyNormal();

  /**
   * @brief getEmptySpecular get Empty Specular Texture
   * @return
   */
  static Texture getEmptySpecular();

  /**
   * @brief getTexture get Texture from image
   * @param path       path to image
   * @return           Texture object uploaded to GPU
   */
  static Texture getTexture(const std::string &path);

  /**
   * @brief getTextureCache Get refrence to cached Texture
   * @return get Texture map
   */
  static CachedTexture &getTextureCache() {
    return textureCache;
  }

private:
  /**
   * @brief uploadTexture Upload texture from CPU to GPU memory.
   * @param data          Pointer to CPU Texture.
   * @param width         Texture width.
   * @param height        Texture height.
   * @return              get Texture Object
   */
  static Texture uploadTexture(const unsigned char *data, unsigned int width,
                               unsigned int height);

  static CachedTexture textureCache; /**< Cached Texture Object*/
};

} /* namespace radix */

#endif /* RADIX_TEXTURELOADER_HPP */
