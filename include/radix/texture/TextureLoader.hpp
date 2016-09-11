#ifndef RADIX_TEXTURELOADER_HPP
#define RADIX_TEXTURELOADER_HPP

#include <map>
#include <string>

#include <radix/texture/Texture.hpp>

namespace radix {

class TextureLoader {
public:
  static Texture getEmptyTexture(const std::string &name,
    const char *pixel = "\xFF\xFF\xFF");
  static Texture getEmptyDiffuse();
  static Texture getEmptyNormal();
  static Texture getEmptySpecular();
  static Texture getTexture(const std::string &path);

  static std::map<std::string, Texture>& getTextureCache() {
    return textureCache;
  }
private:
  static Texture uploadTexture(const unsigned char *data, int width, int height);
  static std::map<std::string, Texture> textureCache;
};

} /* namespace radix */

#endif /* RADIX_TEXTURELOADER_HPP */
