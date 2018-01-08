#ifndef IMGUI_RENDERER_HPP
#define IMGUI_RENDERER_HPP

#include <radix/renderer/SubRenderer.hpp>
//#include <radix/Window.hpp>

struct SDL_Window;
union  SDL_Event;
struct ImDrawData;

namespace radix {

class ImguiRenderer : public SubRenderer {
public:
  ImguiRenderer(World &w, Renderer& ren);

  void shutdown();

  void render() override;

  void newFrame();

  bool isFocus() const;

  bool processEvent(SDL_Event* event);

private:
  void renderDrawLists(ImDrawData* draw_data);

  bool createDeviceObjects();

  void createFontsTexture();

  SDL_Window* window = nullptr;

  uint attribLocationTex = 0, attribLocationProjMtx = 0,
  attribLocationPosition = 0, attribLocationUV = 0,
  attribLocationColor = 0;
  float mouseWheel = 0.0f;
  double mTime = 1.0;
  bool mousePressed[3] = {false, false, false};
  uint vaoHandle = 0, vboHandle = 0, elementsHandle = 0;
  uint shaderHandle = 0, vertHandle = 0, fragHandle = 0;
  uint fontTexture = 0;
};

}
#endif //!IMGUI_RENDERER_HPP
