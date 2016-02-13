#include <radix/renderer/TerminalRenderer.hpp>

#include <cstdio>
#include <vector>
#include <string>

#include <radix/renderer/Renderer.hpp>
#include <radix/model/MeshLoader.hpp>
#include <radix/texture/TextureLoader.hpp>
#include <radix/text/FontLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>
#include <radix/material/MaterialLoader.hpp>

namespace glPortal {

void TerminalRenderer::render(Renderer &renderer) {
  if (not renderer.getScene()->terminal->enabled) {
    return;
  }
  
  int vpWidth, vpHeight;
  renderer.getViewport()->getSize(&vpWidth, &vpHeight);
  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2, vpHeight-100, -5));
  widget.scale(Vector3f(vpWidth, 200, 1));
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  Shader &sh = ShaderLoader::getShader("color.frag");
  Vector4f screenBackgroundColor = renderer.getScene()->terminal->backgroundColor;
  glUseProgram(sh.handle);
  glUniform4f(sh.uni("color"),
              screenBackgroundColor.x,
              screenBackgroundColor.y,
              screenBackgroundColor.z,
              screenBackgroundColor.w);
  renderer.renderMesh(camera, sh, widget, mesh, nullptr);
  renderer.setFontColor(renderer.getScene()->screen->textColor);
  renderer.setFontSize(1);
  renderer.renderText(camera,
                      Input::getCharBuffer(),
                      Vector3f(0, vpHeight-30, -1));
}

} /* namespace glPortal */
