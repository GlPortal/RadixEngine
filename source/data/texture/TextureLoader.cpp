#include <radix/data/texture/TextureLoader.hpp>

#include <radix/core/gl/OpenGL.hpp>

#include <FreeImagePlus.h>
#include <radix/env/Environment.hpp>

namespace radix {

std::map<std::string, Texture> TextureLoader::textureCache = {};

Texture TextureLoader::getEmptyTexture(const std::string &name,
                                       const char *pixel) {
  // Check for cached texture
  auto it = textureCache.find(name);
  if (it != textureCache.end())
    return it->second;

  // upload texture to GPU
  Texture texture =
      uploadTexture(reinterpret_cast<const unsigned char *>(pixel), 1, 1);
  textureCache.insert(std::make_pair(name, texture)); // Cache texture
  return texture;
}

Texture TextureLoader::getEmptyDiffuse() {
  return TextureLoader::getEmptyTexture("engine@empty/diffuse");
}

Texture TextureLoader::getEmptyNormal() {
  return TextureLoader::getEmptyTexture("engine@empty/normal", "\xFF\x7F\x7F");
}

Texture TextureLoader::getEmptySpecular() {
  return TextureLoader::getEmptyTexture("engine@empty/specular",
                                        "\x7F\x7F\x7F");
}

Texture TextureLoader::getTexture(const std::string &path) {
  // Check if texture is chached
  auto it = textureCache.find(path);
  if (it != textureCache.end())
    return it->second;

  // get image name
  const std::string imagePath = Environment::getDataDir() + "/textures/" + path;
  const char* pImagePath = imagePath.c_str();
  // Read image from hard
  FIBITMAP *bitmap = FreeImage_Load(FreeImage_GetFileType(pImagePath), pImagePath);

  // Convert image to 32Bits
  FIBITMAP *image = FreeImage_ConvertTo32Bits(bitmap);
  FreeImage_Unload(bitmap);                         // Free Temp bitmap
  FreeImage_FlipVertical(image);                    // Flip image vertically
  unsigned int width = FreeImage_GetWidth(image);   // get width image
  unsigned int height = FreeImage_GetHeight(image); // get height image

  // upload texture to GPU
  Texture texture = uploadTexture(FreeImage_GetBits(image), width, height);
  // add new texture to cache
  textureCache.insert(std::make_pair(path, texture));

  FreeImage_Unload(image); // free created image

  return texture;
}

Texture TextureLoader::uploadTexture(const unsigned char *data,
                                     unsigned int width, unsigned int height) {
  GLuint handle;
  glGenTextures(1, &handle); // Create Texture OpenGL handler
  glBindTexture(GL_TEXTURE_2D,
                handle); // Bind and Set OpenGL Texture type 2D Texture
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // sets pixel storage modes
  // Allocate texture with (width x height x 4) and copy data from CPU to GPU
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(width),
               static_cast<int>(height), 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
  // Set texture Filters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  glGenerateMipmap(GL_TEXTURE_2D); // Generate Mipmap
  glBindTexture(GL_TEXTURE_2D, 0); // unbind 2D Texture

  // Create Texture Object
  return Texture(handle, static_cast<int>(width), static_cast<int>(height));
}

} /* namespace radix */
