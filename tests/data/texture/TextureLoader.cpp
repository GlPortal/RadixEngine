#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <glad.h>
#include <catch2/catch.hpp>
#include "radix/data/texture/TextureLoader.hpp"


static bool g_bPassed  = false;
static bool g_bFopen   = true;
static int  g_iDefault = 0;
enum class TextureType : int {
  Diffuse,
  Normal,
  Specular,
  Overflow
};
static const char* g_pBuffers[] = {
  "\xFF\xFF\xFF",     // diffuse
  "\x7F\x7F\xFF",     // normal
  "\x7F\x7F\x7F",     // specular
  "\x7F\x7F\x7F\xFF", // overflow
};

#ifdef __cplusplus
extern "C" {
#endif //! __cplusplus

#include <string.h>
FILE *__wrap_fopen(const char *path, const char *mode);
extern FILE *__real_fopen(const char *path, const char *mode);

FILE *__wrap_fopen(const char *path, const char *mode) {
  if(g_bFopen) {
    return __real_fopen(path, mode);
  }
  return 0;
}

PFNGLGENTEXTURESPROC glad_glGenTextures;
void glad_glGenTextures_(GLsizei n, GLuint *textures) {
  *textures = 1;
}
PFNGLBINDTEXTUREPROC glad_glBindTexture;
void glad_glBindTexture_(GLenum target, GLuint texture) {
}
PFNGLPIXELSTOREIPROC glad_glPixelStorei;
void glad_glPixelStorei_(GLenum pname, GLint param) {
}
PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
void glad_glTexParameteri_(GLenum target, GLenum pname, GLint param) {
}
PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
void glad_glTexImage2D_(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
  if(!pixels) {
    g_bPassed = false;
  } else {
    g_bPassed = strcmp(g_pBuffers[g_iDefault],
                       reinterpret_cast<const char*>(pixels)) == 0;
  }
}
PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
void glad_glGenerateMipmap_(GLenum target) {
}

#ifdef __cplusplus
}
#endif //! __cplusplus

struct TextureLoaderFixtires {
  TextureLoaderFixtires() {
    glGenTextures    = glad_glGenTextures_;
    glBindTexture    = glad_glBindTexture_;
    glPixelStorei    = glad_glPixelStorei_;
    glTexParameteri  = glad_glTexParameteri_;
    glTexImage2D     = glad_glTexImage2D_;
    glGenerateMipmap = glad_glGenerateMipmap_;
  }
  ~TextureLoaderFixtires() {}
};

TEST_CASE_METHOD(TextureLoaderFixtires, "Load Texture", "[texture-loader]") {
  g_bFopen = false;
  SECTION("getEmptyTextureDiffuse") {
    g_iDefault = static_cast<int>(TextureType::Diffuse);
    auto texture = radix::TextureLoader::getEmptyDiffuse();
    REQUIRE(g_bPassed);
    REQUIRE(texture.width  == 1);
    REQUIRE(texture.height == 1);
  }
  g_bPassed = false;
  SECTION("getEmptyTextureNormal") {
    g_iDefault = static_cast<int>(TextureType::Normal);
    auto texture = radix::TextureLoader::getEmptyNormal();
    REQUIRE(g_bPassed);
    REQUIRE(texture.width  == 1);
    REQUIRE(texture.height == 1);
  }
  g_bPassed = false;
  SECTION("getEmptyTextureSpecular") {
    g_iDefault = static_cast<int>(TextureType::Specular);
    auto texture = radix::TextureLoader::getEmptySpecular();
    REQUIRE(g_bPassed);
    REQUIRE(texture.width  == 1);
    REQUIRE(texture.height == 1);
  }
  g_bPassed = false;
  SECTION("Pass nullptr to getEmptyTexture") {
    auto texture = radix::TextureLoader::getEmptyTexture("engine@empty/nullptr", nullptr);
    REQUIRE(!g_bPassed);
    REQUIRE(texture.width  == 0);
    REQUIRE(texture.height == 0);
  }
  g_bPassed = false;
  SECTION("Pass pointer greater than 3 pixels to getEmptyTexture") {
    g_iDefault = static_cast<int>(TextureType::Overflow);
    auto ptr   = g_pBuffers[g_iDefault];
    auto texture = radix::TextureLoader::getEmptyTexture("engine@empty/overflow", ptr);
    REQUIRE(!g_bPassed);
    REQUIRE(texture.width  == 0);
    REQUIRE(texture.height == 0);
  }
  g_bPassed = false;
  SECTION("Pass empty tag to getEmptyTexture") {
    g_iDefault = static_cast<int>(TextureType::Diffuse);
    auto ptr   = g_pBuffers[g_iDefault];
    auto texture = radix::TextureLoader::getEmptyTexture("", ptr);
    REQUIRE(!g_bPassed);
    REQUIRE(texture.width  == 0);
    REQUIRE(texture.height == 0);
  }
}
