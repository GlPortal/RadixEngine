#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <glad/glad.h>

#include <radix/env/Util.hpp>
#include <radix/env/Environment.hpp>

#include <radix/core/math/Vector2i.hpp>
#include <radix/data/texture/SpritSheetLoader.hpp>
#include <radix/data/texture/TextureLoader.hpp>


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
  }
  PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
  void glad_glGenerateMipmap_(GLenum target) {}

#ifdef __cplusplus
}
#endif  //! __cplusplus

struct TextureLoaderFixtires {
  TextureLoaderFixtires() {
    radix::Util::Init();
    radix::Environment::setDataDir(".");

    glGenTextures = glad_glGenTextures_;
    glBindTexture = glad_glBindTexture_;
    glPixelStorei = glad_glPixelStorei_;
    glTexParameteri = glad_glTexParameteri_;
    glTexImage2D = glad_glTexImage2D_;
    glGenerateMipmap = glad_glGenerateMipmap_;
  }

  ~TextureLoaderFixtires() = default;
};

TEST_CASE_METHOD(TextureLoaderFixtires, "test_set_frame_count") {
  radix::SpritSheetLoader loader;
  const std::string textureID = "animation_1024.png";
  loader.load(textureID);
  loader.setSpritSize(radix::Vector2i{8, 4});
  const auto spritSheet = loader.create();
  const auto framesCount = spritSheet.getFramesCount();

  printf("%d == %d\n", framesCount, 8*4);
  REQUIRE(framesCount == 8*4);
}

TEST_CASE_METHOD(TextureLoaderFixtires, "test_set_frame_to_zero") {
  radix::SpritSheetLoader loader;
  const std::string textureID = "animation_1024.png";
  loader.load(textureID);
  loader.setSpritSize(radix::Vector2i{8, 4});
  auto spritSheet = loader.create();
  const auto frame = spritSheet[0];

  REQUIRE(std::abs(frame.x - 0.f/1024.f) < 0.001f);
  REQUIRE(std::abs(frame.y - 0.f/1024.f) < 0.001f);

  REQUIRE(std::abs(frame.w - 128.f/1024.f) < 0.001f);
  REQUIRE(std::abs(frame.h - 256.f/1024.f) < 0.001f);
}

TEST_CASE_METHOD(TextureLoaderFixtires, "test_set_frame_to_one") {
  radix::SpritSheetLoader loader;
  const std::string textureID = "animation_1024.png";
  loader.load(textureID);
  loader.setSpritSize(radix::Vector2i{8, 4});
  auto spritSheet = loader.create();
  const auto frame = spritSheet[1];

  printf("frame (%f, %f)\n", frame.x, frame.y);

  REQUIRE(std::abs(frame.x - 128.f/1024.f) < 0.001f);
  REQUIRE(std::abs(frame.y - 0.f/1024.f)   < 0.001f);

  REQUIRE(std::abs(frame.w - 128.f/1024.f) < 0.001f);
  REQUIRE(std::abs(frame.h - 256.f/1024.f) < 0.001f);
}

TEST_CASE_METHOD(TextureLoaderFixtires, "test_set_frame_to_7") {
  radix::SpritSheetLoader loader;
  const std::string textureID = "animation_1024.png";
  loader.load(textureID);
  loader.setSpritSize(radix::Vector2i{8, 4});
  auto spritSheet = loader.create();
  const auto frame = spritSheet[7];

  REQUIRE(std::abs(frame.x - ((1024.f - 128.f) / 1024.f)) < 0.001f);
  REQUIRE(std::abs(frame.y - 0.f/1024.f) < 0.001f);

  REQUIRE(std::abs(frame.w - 128.f/1024.f) < 0.001f);
  REQUIRE(std::abs(frame.h - 256.f/1024.f) < 0.001f);
}

TEST_CASE_METHOD(TextureLoaderFixtires, "test_set_frame_to_8") {
  radix::SpritSheetLoader loader;
  const std::string textureID = "animation_1024.png";
  loader.load(textureID);
  loader.setSpritSize(radix::Vector2i{8, 4});
  auto spritSheet = loader.create();
  const auto frame = spritSheet[8];

  printf("frame (%f, %f)\n", frame.x, frame.y);

  REQUIRE(std::abs(frame.x - 0.f  /1024.f)   < 0.001f);
  REQUIRE(std::abs(frame.y - 256.f/1024.f) < 0.001f);

  REQUIRE(std::abs(frame.w - 128.f/1024.f) < 0.001f);
  REQUIRE(std::abs(frame.h - 256.f/1024.f) < 0.001f);
}

TEST_CASE_METHOD(TextureLoaderFixtires, "test_set_frame_to_24") {
  radix::SpritSheetLoader loader;
  const std::string textureID = "animation_1024.png";
  loader.load(textureID);
  loader.setSpritSize(radix::Vector2i{8, 4});
  auto spritSheet = loader.create();
  const auto frame = spritSheet[24];

  REQUIRE(std::abs(frame.x - (0 / 1024.f)) < 0.001f);
  REQUIRE(std::abs(frame.y - ((1024.f - 256.f) / 1024.f)) < 0.001f);

  REQUIRE(std::abs(frame.w - 128.f/1024.f) < 0.001f);
  REQUIRE(std::abs(frame.h - 256.f/1024.f) < 0.001f);
}

TEST_CASE_METHOD(TextureLoaderFixtires, "test_set_frame_to_31") {
  radix::SpritSheetLoader loader;
  const std::string textureID = "animation_1024.png";
  loader.load(textureID);
  loader.setSpritSize(radix::Vector2i{8, 4});
  auto spritSheet = loader.create();
  const auto frame = spritSheet[31];

  REQUIRE(std::abs(frame.x - ((1024.f - 128.f) / 1024.f)) < 0.001f);
  REQUIRE(std::abs(frame.y - ((1024.f - 256.f) / 1024.f)) < 0.001f);

  REQUIRE(std::abs(frame.w - 128.f/1024.f) < 0.001f);
  REQUIRE(std::abs(frame.h - 256.f/1024.f) < 0.001f);
}

SCENARIO("Pass texture path to get full loaded spritSheet",
         "[sprit-sheet,texture-loader]") {
  TextureLoaderFixtires fix;
  radix::SpritSheetLoader loader;

  GIVEN("A texture not exist") {
    WHEN("pass empty string") {
      loader.load("");
      THEN("the full size is 0x0") {
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.fullWidth()  == 0);
        REQUIRE(spritSheet.fullHeight() == 0);
      }
    }
  }

  GIVEN("A texture/animation_512.png exist") {
    const std::string textureID = "animation_512.png";
    WHEN("pass texture/animation_512.png") {
      loader.load(textureID);
      THEN("the full size is 512x512") {
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.fullWidth()  == 512);
        REQUIRE(spritSheet.fullHeight() == 512);

        REQUIRE(spritSheet.width()  == 512);
        REQUIRE(spritSheet.height() == 512);
      }

      THEN("set sprit size 7x4") {
        loader.setSpritSize(radix::Vector2i{7, 4});
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.width()  == 73);
        REQUIRE(spritSheet.height() == 128);
      }
    }
  }
  /*
  */

  GIVEN("A texture/animation_1024.png exist") {
    const std::string textureID = "animation_1024.png";
    WHEN("pass texture/animation_1024.png") {
      loader.load(textureID);
      THEN("the full size is 1024x1024") {
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.fullWidth()  == 1024);
        REQUIRE(spritSheet.fullHeight() == 1024);
      }

      WHEN("set sprit size 7x4") {
        const auto gridSize = radix::Vector2i{146, 256};
        loader.setSpritSize({7, 4});
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.width()  == gridSize.width);
        REQUIRE(spritSheet.height() == gridSize.height);
      }
    }
  }
}
