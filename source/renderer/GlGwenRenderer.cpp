#define NOMINMAX

#include <radix/renderer/GlGwenRenderer.hpp>

#include <cmath>
#include <fstream>
#include <iterator>

#include <Gwen/Texture.h>
#include <Gwen/WindowProvider.h>

#include <FreeImage.h>
#include <radix/core/gl/VBO.hpp>
#include <radix/core/math/Matrix4f.hpp>
#include <radix/env/Util.hpp>
#include <radix/env/Environment.hpp>
#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/data/texture/TextureLoader.hpp>

namespace radix {

GlGwenRenderer::GlGwenRenderer() :
  fontLetterSpacing(1.0f / 16.0f),
  fontSpacing(0xFF) {
  vertNum = 0;
  fontScale[0] = fontScale[1] = 1.5f;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  vbo = std::make_unique<VBO>(MaxVerts*sizeof(Vertex), VBO::Dynamic | VBO::Draw);
  vbo->bind();
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), (GLvoid*)offsetof(Vertex, x));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), (GLvoid*)offsetof(Vertex, u));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,
    sizeof(Vertex), (GLvoid*)offsetof(Vertex, r));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);
}

GlGwenRenderer::~GlGwenRenderer() {
  glDeleteVertexArrays(1, &vao);
}

void GlGwenRenderer::Init() {
  Util::Log(Debug, "GlGwenRenderer") << "Initialize";
  loadDebugFont();
}

void GlGwenRenderer::Begin() {
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

void GlGwenRenderer::End() {
  flush();
}

void GlGwenRenderer::flush() {
  if (vertNum == 0) {
    return;
  }

  Shader &sh = ShaderLoader::getShader("2d.frag", "2d.vert");
  sh.bind();

  glUniform1i(sh.uni("tex"), 0);

  Matrix4f projMatrix;
  // TODO move this into a generic ortho matrix method
  float left, right, bottom, top, far = 1.f, near = -1.f;
  {
    GLint view[4];
    glGetIntegerv(GL_VIEWPORT, &view[0]);
    left = view[0];
    top = view[1];
    right = view[2];
    bottom = view[3];
  }
  projMatrix[0]  =  2/(right-left);
  projMatrix[5]  =  2/(top-bottom);
  projMatrix[10] = -2/(far-near);
  projMatrix[12] = -(right+left)/(right-left);
  projMatrix[13] = -(top+bottom)/(top-bottom);
  projMatrix[14] = -(far+near)/(far-near);

  glUniformMatrix4fv(sh.uni("projectionMatrix"), 1, false, projMatrix.toArray());

  glBindVertexArray(vao);

  vbo->update(vertices, vertNum);

  glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertNum);

  sh.release();

  vertNum = 0;
}

void GlGwenRenderer::addVert(int x, int y, float u, float v) {
  if (vertNum >= MaxVerts - 1) {
    flush();
  }

  vertices[vertNum].x = (short) x;
  vertices[vertNum].y = (short) y;
  vertices[vertNum].u = u;
  vertices[vertNum].v = v;
  vertices[vertNum].r = color.r;
  vertices[vertNum].g = color.g;
  vertices[vertNum].b = color.b;
  vertices[vertNum].a = color.a;
  vertNum++;
}

void GlGwenRenderer::DrawFilledRect(Gwen::Rect rect) {
  GLuint boundtex;
  GLboolean texturesOn;
  glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundtex);
  GLuint glTexEmptyHandle = TextureLoader::getEmptyDiffuse().handle;

  if (not texturesOn or boundtex != glTexEmptyHandle) {
    flush();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexEmptyHandle);
    glEnable(GL_TEXTURE_2D);
  }

  Translate(rect);
  addVert(rect.x, rect.y);
  addVert(rect.x + rect.w, rect.y);
  addVert(rect.x, rect.y + rect.h);
  addVert(rect.x + rect.w, rect.y);
  addVert(rect.x + rect.w, rect.y + rect.h);
  addVert(rect.x, rect.y + rect.h);
}

void GlGwenRenderer::SetDrawColor(Gwen::Color color) {
  this->color = color;
}

void GlGwenRenderer::StartClip() {
  flush();
  Gwen::Rect rect = ClipRegion();
  // OpenGL's scissor feature speaks window coordinates,
  // which start from the bottom left.
  {
    struct {
      GLint x, y, w, h;
    } view;
    glGetIntegerv(GL_VIEWPORT, (GLint*)&view);
    rect.x = std::max(rect.x, view.x);
    rect.y = std::max(view.h - (rect.y + rect.h), view.y);
    rect.w = std::min(rect.w, view.w);
    rect.h = std::min(rect.h, view.h);
  }
  glScissor(rect.x * Scale(), rect.y * Scale(), rect.w * Scale(), rect.h * Scale());
  glEnable(GL_SCISSOR_TEST);
};

void GlGwenRenderer::EndClip() {
  flush();
  glDisable(GL_SCISSOR_TEST);
};

void GlGwenRenderer::DrawTexturedRect(Gwen::Texture *tex, Gwen::Rect rect, float u1, float v1,
  float u2, float v2) {
  GLuint *glTex = (GLuint*)tex->data;

  // Missing image, not loaded properly?
  if (not glTex) {
    return DrawMissingImage(rect);
  }

  Translate(rect);
  GLuint boundtex;
  GLboolean texturesOn;
  glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundtex);

  // Switch textures if necessary - that means drawing what we've accumulated so far
  if (not texturesOn or *glTex != boundtex) {
    flush();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *glTex);
    glEnable(GL_TEXTURE_2D);
  }

  addVert(rect.x, rect.y, u1, v1);
  addVert(rect.x + rect.w, rect.y, u2, v1);
  addVert(rect.x, rect.y + rect.h, u1, v2);
  addVert(rect.x + rect.w, rect.y, u2, v1);
  addVert(rect.x + rect.w, rect.y + rect.h, u2, v2);
  addVert(rect.x, rect.y + rect.h, u1, v2);
}

void GlGwenRenderer::LoadTexture(Gwen::Texture *tex) {
  int width = 0, height = 0;
  FIBITMAP *bitmap = FreeImage_Load(FreeImage_GetFileType(tex->name.c_str(), 0), tex->name.c_str());
  FIBITMAP *image = FreeImage_ConvertTo32Bits(bitmap);
  width = FreeImage_GetWidth(image);
  height = FreeImage_GetHeight(image);
  if (width == 0 or height == 0) {
    tex->failed = true;
    Util::Log(Debug, "GlGwenRenderer") << "LoadTexture: loading " << tex->name.c_str() << " failed";
    return;
  }
  tex->width = width;
  tex->height = height;

  // Create the opengl texture
  GLuint *glTex = new GLuint;
  tex->data = glTex;
  glGenTextures(1, glTex);
  glBindTexture(GL_TEXTURE_2D, *glTex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*) FreeImage_GetBits(image));
  FreeImage_Unload(image);
  FreeImage_Unload(bitmap);
  Util::Log(Debug, "GlGwenRenderer") << "LoadTexture " << tex->name.c_str() << ", id " << *glTex;
}

void GlGwenRenderer::FreeTexture(Gwen::Texture *tex) {
  GLuint *glTex = (GLuint*)tex->data;

  if (not glTex) {
    return;
  }

  glDeleteTextures(1, glTex);
  delete glTex;
  tex->data = nullptr;
}

Gwen::Color GlGwenRenderer::PixelColour(Gwen::Texture *tex, unsigned int x, unsigned int y,
  const Gwen::Color &col_default) {
  GLuint *glTex = (GLuint*)tex->data;

  if (not glTex) {
    return col_default;
  }

  unsigned int pxSize = sizeof(unsigned char) * 4;
  glBindTexture(GL_TEXTURE_2D, *glTex);
  unsigned char* data = (unsigned char*) malloc(pxSize * tex->width * tex->height);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  unsigned int offset = (y * tex->width + x) * pxSize;
  Gwen::Color c;
  c.r = data[0 + offset];
  c.g = data[1 + offset];
  c.b = data[2 + offset];
  c.a = data[3 + offset];
  //
  // Retrieving the entire texture for a single pixel read
  // is kind of a waste - maybe cache this pointer in the texture
  // data and then release later on? It's never called during runtime
  // - only during initialization.
  //
  free(data);
  return c;
}

void GlGwenRenderer::loadDebugFont() {
  if (fontTex) {
    return;
  }

  fontTex = std::make_unique<Gwen::Texture>();
  fontTex->Load(Environment::getDataDir() + "/gui/DebugFont.png", this);

  std::ifstream f(Environment::getDataDir() + "/gui/DebugFont.spacing", std::ios::binary);
  // 9-width chars are skipped else, because 0x09 = <tab>, which is whitespace.
  // See http://en.cppreference.com/w/cpp/iterator/istream_iterator#Notes .
  // So, disable this behaviour.
  f.unsetf(std::ios_base::skipws);
  std::istream_iterator<char> start(f), end;
  fontSpacing.assign(start, end);
}

void GlGwenRenderer::RenderText(Gwen::Font *font, Gwen::Point pos, const Gwen::UnicodeString &text) {
  float size = font->size * Scale();

  if (not text.length()) {
    return;
  }

  Gwen::String converted_string = Gwen::Utility::UnicodeToString( text );
  float xOffset = 0.0f;

  for (unsigned int i = 0; i < text.length(); ++i) {
    char ch = converted_string[i];
    float curSpacing = fontSpacing[ch] * fontLetterSpacing * size * fontScale[0];
    Gwen::Rect r(pos.x + xOffset, pos.y - size * 0.5, size * fontScale[0], size * fontScale[1]);

    if (fontTex) {
      float uv_texcoords[4] = {0., 0., 1., 1.};

      if (ch >= 0) {
        float cx = ( ch % 16 ) / 16.0;
        float cy = ( ch / 16 ) / 16.0;
        uv_texcoords[0] = cx;
        uv_texcoords[1] = cy;
        uv_texcoords[2] = float( cx + 1.0f / 16.0f );
        uv_texcoords[3] = float( cy + 1.0f / 16.0f );
      }

      DrawTexturedRect(fontTex.get(), r, uv_texcoords[0], uv_texcoords[3], uv_texcoords[2], uv_texcoords[1]);
      xOffset += curSpacing;
    } else {
      DrawFilledRect(r);
      xOffset += curSpacing;
    }
  }
}

Gwen::Point GlGwenRenderer::MeasureText(Gwen::Font *font, const Gwen::UnicodeString &text) {
  Gwen::Point p;
  float size = font->size * Scale();
  Gwen::String converted_string = Gwen::Utility::UnicodeToString(text);
  float spacing = 0.0f;

  for (int i = text.length()-1; i >= 0; --i) {
    char ch = converted_string[i];
    spacing += fontSpacing[ch];
  }

  p.x = spacing * fontLetterSpacing * size * fontScale[0];
  p.y = font->size * Scale();
  return p;
}

} /* namespace radix */
