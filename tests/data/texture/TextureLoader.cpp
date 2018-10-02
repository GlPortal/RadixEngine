#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <glad.h>
#include <catch2/catch.hpp>
#include "radix/data/texture/TextureLoader.hpp"

static bool g_bFopen = true;

#ifdef __cplusplus
extern "C" {
#endif //! __cplusplus

FILE *__wrap_fopen(const char *path, const char *mode);
extern FILE *__real_fopen(const char *path, const char *mode);

FILE *__wrap_fopen(const char *path, const char *mode) {
  if(g_bFopen) {
    printf("Inside\n");
    return __real_fopen(path, mode);
  }
  printf("Outside\n");
  return 0;
}

PFNGLGENTEXTURESPROC glad_glGenTextures;
void glad_glGenTextures_(GLsizei n, GLuint *textures) {
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
  SECTION("getEmptyTexture") {
    g_bFopen = false;
    auto texture = radix::TextureLoader::getEmptyTexture("engine@empty/diffuse");
  }
}
