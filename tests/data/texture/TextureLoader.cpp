#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <glad/glad.h>
#include "radix/data/texture/TextureLoader.hpp"
#include <radix/env/Util.hpp>
#include <radix/env/Environment.hpp>

static bool g_bPassed = false;
static int g_iDefault = 0;
enum class TextureType : int { Diffuse, Normal, Specular, Overflow };
static const char *g_pBuffers[] = {
    "\xFF\xFF\xFF",      // diffuse
    "\x7F\x7F\xFF",      // normal
    "\x7F\x7F\x7F",      // specular
    "\x7F\x7F\x7F\xFF",  // overflow
};

#ifdef __cplusplus
extern "C" {
#endif  //! __cplusplus

PFNGLGENTEXTURESPROC glad_glGenTextures;
void glad_glGenTextures_(GLsizei n, GLuint *textures) { *textures = 1; }
PFNGLBINDTEXTUREPROC glad_glBindTexture;
void glad_glBindTexture_(GLenum target, GLuint texture) {}
PFNGLPIXELSTOREIPROC glad_glPixelStorei;
void glad_glPixelStorei_(GLenum pname, GLint param) {}
PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
void glad_glTexParameteri_(GLenum target, GLenum pname, GLint param) {}
PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
void glad_glTexImage2D_(GLenum target, GLint level, GLint internalformat,
			GLsizei width, GLsizei height, GLint border,
			GLenum format, GLenum type, const void *pixels) {
  if (!pixels) {
    g_bPassed = false;
  } else {
    g_bPassed = strcmp(g_pBuffers[g_iDefault],
		       reinterpret_cast<const char *>(pixels)) == 0;
  }
}
PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
void glad_glGenerateMipmap_(GLenum target) {}

#ifdef __cplusplus
}
#endif  //! __cplusplus

struct TextureLoaderFixtires {
  TextureLoaderFixtires() {
    glGenTextures = glad_glGenTextures_;
    glBindTexture = glad_glBindTexture_;
    glPixelStorei = glad_glPixelStorei_;
    glTexParameteri = glad_glTexParameteri_;
    glTexImage2D = glad_glTexImage2D_;
    glGenerateMipmap = glad_glGenerateMipmap_;
  }
  ~TextureLoaderFixtires() {}
};

TEST_CASE_METHOD(TextureLoaderFixtires, "Load Texture", "[texture-loader]") {
  SECTION("getEmptyTextureDiffuse") {
    g_iDefault = static_cast<int>(TextureType::Diffuse);
    auto texture = radix::TextureLoader::getEmptyDiffuse();
    REQUIRE(g_bPassed);
    REQUIRE(texture.width == 1);
    REQUIRE(texture.height == 1);
  }
  g_bPassed = false;
  SECTION("getEmptyTextureNormal") {
    g_iDefault = static_cast<int>(TextureType::Normal);
    auto texture = radix::TextureLoader::getEmptyNormal();
    REQUIRE(g_bPassed);
    REQUIRE(texture.width == 1);
    REQUIRE(texture.height == 1);
  }
  g_bPassed = false;
  SECTION("getEmptyTextureSpecular") {
    g_iDefault = static_cast<int>(TextureType::Specular);
    auto texture = radix::TextureLoader::getEmptySpecular();
    REQUIRE(g_bPassed);
    REQUIRE(texture.width == 1);
    REQUIRE(texture.height == 1);
  }
  SECTION("getEmptyTexture") {
    g_bPassed = false;
    SECTION("Pass nullptr to getEmptyTexture") {
      auto texture = radix::TextureLoader::getEmptyTexture(
	  "engine@empty/nullptr", nullptr);
      REQUIRE(!g_bPassed);
      REQUIRE(texture.width == 0);
      REQUIRE(texture.height == 0);
    }
    g_bPassed = false;
    SECTION("Pass pointer greater than 3 pixels to getEmptyTexture") {
      g_iDefault = static_cast<int>(TextureType::Overflow);
      auto ptr = g_pBuffers[g_iDefault];
      auto texture =
	  radix::TextureLoader::getEmptyTexture("engine@empty/overflow", ptr);
      REQUIRE(!g_bPassed);
      REQUIRE(texture.width == 0);
      REQUIRE(texture.height == 0);
    }
    g_bPassed = false;
    SECTION("Pass empty tag to getEmptyTexture") {
      g_iDefault = static_cast<int>(TextureType::Diffuse);
      auto ptr = g_pBuffers[g_iDefault];
      auto texture = radix::TextureLoader::getEmptyTexture("", ptr);
      REQUIRE(!g_bPassed);
      REQUIRE(texture.width == 0);
      REQUIRE(texture.height == 0);
    }
  }
  SECTION("getTexture") {
    radix::Util::Init();
    radix::Environment::setDataDir(".");
    SECTION("Pass empty file to getTexture") {
      auto texture = radix::TextureLoader::getTexture("");
      REQUIRE(!g_bPassed);
      REQUIRE(texture.width == 0);
      REQUIRE(texture.height == 0);
    }
    SECTION("Pass wrong file to getTexture") {
      auto texture = radix::TextureLoader::getTexture("asd");
      REQUIRE(!g_bPassed);
      REQUIRE(texture.width == 0);
      REQUIRE(texture.height == 0);
    }
    SECTION("Pass 1 pixel file to getTexture") {
      auto texture = radix::TextureLoader::getTexture("onePixel.png");
      REQUIRE(texture.width  == 1);
      REQUIRE(texture.height == 1);
    }
  }
}
