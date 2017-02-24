#ifndef RADIX_GWEN_RENDERER_HPP
#define RADIX_GWEN_RENDERER_HPP

#include <memory>
#include <vector>

#include <Gwen/Gwen.h>
#include <Gwen/BaseRender.h>

#include <radix/core/gl/OpenGL.hpp>

#ifdef _WIN32
  #undef near
  #undef far
  #include <ciso646>
#endif

namespace radix {

class VBO;

class GlGwenRenderer : public Gwen::Renderer::Base {
public:
  struct Vertex {
    float x, y;
    float u, v;
    unsigned char r, g, b, a;
  };

  GlGwenRenderer();
  ~GlGwenRenderer();

  void Init() final override;

  void Begin() final override;
  void End() final override;

  void SetDrawColor(Gwen::Color color) final override;
  void DrawFilledRect(Gwen::Rect rect) final override;

  void StartClip() final override;
  void EndClip() final override;

  void DrawTexturedRect(Gwen::Texture *tex, Gwen::Rect rect, float u1 = 0.0f,
    float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) final override;
  void LoadTexture(Gwen::Texture *tex) final override;
  void FreeTexture(Gwen::Texture *tex) final override;
  Gwen::Color PixelColour(Gwen::Texture *tex, unsigned int x, unsigned int y,
    const Gwen::Color &col_default) final override;

  void RenderText(Gwen::Font *font, Gwen::Point pos, const Gwen::UnicodeString &text) final override;
  Gwen::Point MeasureText(Gwen::Font *font, const Gwen::UnicodeString &text) final override;

protected:

  static const int MaxVerts = 1024;

  void flush();
  void addVert(int x, int y, float u = 0.0f, float v = 0.0f);
  void loadDebugFont();

  GLuint vao;
  std::unique_ptr<VBO> vbo;

  std::unique_ptr<Gwen::Texture> fontTex;
  float fontScale[2];
  float fontLetterSpacing;
  std::vector<char> fontSpacing;

  Gwen::Color color;
  int vertNum;
  Vertex vertices[MaxVerts];
};

} /* namespace radix */

#endif /* RADIX_GWEN_RENDERER_HPP */
