#include <radix/texture/TextureLoader.hpp>

#include <cstdio>
#include <utility>

#include <epoxy/gl.h>

#include <radix/env/Environment.hpp>
#include <FreeImagePlus.h>

namespace radix {

std::map<std::string, Texture> TextureLoader::textureCache = {};

Texture TextureLoader::getEmptyTexture(const std::string &name,
  const char *pixel, int pxDepth) {
  auto it = textureCache.find(name);
  if (it != textureCache.end()) {
    return it->second;
  }
  Texture texture = uploadTexture((const unsigned char*)pixel, 1, 1, pxDepth);
  texture.width = 1;
  texture.height = 1;
  textureCache.insert(std::pair<std::string, Texture>(name, texture));
  return texture;
}

Texture TextureLoader::getEmptyDiffuse() {
  return TextureLoader::getEmptyTexture("engine@empty/diffuse");
}

Texture TextureLoader::getEmptyNormal() {
  return TextureLoader::getEmptyTexture("engine@empty/normal", "\x7F\x7F\xFF");
}

Texture TextureLoader::getEmptySpecular() {
  return TextureLoader::getEmptyTexture("engine@empty/specular", "\x7F\x7F\x7F");
}

Texture TextureLoader::getTexture(const std::string &path) {
  auto it = textureCache.find(path);
  if (it != textureCache.end()) {
    return it->second;
  }
  int width = 0, height = 0, bytes = 0;
  FIBITMAP *bitmap = FreeImage_Load(FreeImage_GetFileType((Environment::getDataDir() + "/textures/" + path).c_str()), (Environment::getDataDir() + "/textures/" + path).c_str());
  FIBITMAP *image = FreeImage_ConvertTo32Bits(bitmap);
  FreeImage_FlipVertical(image);
  width = FreeImage_GetWidth(image);
  height = FreeImage_GetHeight(image);
  bytes = 4;
  Texture texture = uploadTexture(FreeImage_GetBits(image), width, height, bytes);
  texture.width = width;
  texture.height = height;
  textureCache.insert(std::pair<std::string, Texture>(path, texture));
  FreeImage_Unload(image);
  FreeImage_Unload(bitmap);
  return texture;
}

Texture TextureLoader::uploadTexture(const unsigned char *data, int width, int height, int bytes) {
  Texture texture;
  GLuint handle;
  glGenTextures(1, &handle);
  glBindTexture(GL_TEXTURE_2D, handle);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  if (bytes == 3) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  } else if (bytes == 4) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  texture.handle = handle;

  return texture;
}

} /* namespace radix */
