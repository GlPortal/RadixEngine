#include <radix/renderer/ImguiRenderer.hpp>

#include <SDL2/SDL.h>
#include <radix/core/gl/OpenGL.hpp>
#include <radix/World.hpp>
#include <radix/BaseGame.hpp>
#include <imgui/imgui.h>

namespace radix {

ImguiRenderer::ImguiRenderer(World& world, Renderer& renderer)
    : SubRenderer(world, renderer) {
  window = world.game.getWindow().window;

  ImGuiIO& io = ImGui::GetIO();
  // Keyboard mapping. ImGui will use those indices to peek into the
  // io.KeyDown[] array.
  io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
  io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
  io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
  io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
  io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
  io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
  io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
  io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
  io.KeyMap[ImGuiKey_A] = SDLK_a;
  io.KeyMap[ImGuiKey_C] = SDLK_c;
  io.KeyMap[ImGuiKey_V] = SDLK_v;
  io.KeyMap[ImGuiKey_X] = SDLK_x;
  io.KeyMap[ImGuiKey_Y] = SDLK_y;
  io.KeyMap[ImGuiKey_Z] = SDLK_z;
  // Alternatively you can set this to NULL and call ImGui::GetDrawData()
  // after ImGui::Render() to get the same ImDrawData pointer.
  io.RenderDrawListsFn = nullptr;
  io.SetClipboardTextFn = [](void*, const char* text) -> void {
    SDL_SetClipboardText(text);
  };
  io.GetClipboardTextFn = [](void*) -> const char* {
    return SDL_GetClipboardText();
  };
  io.ClipboardUserData = nullptr;

#ifdef _WIN32
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(window, &wmInfo);
  io.ImeWindowHandle = wmInfo.info.win.window;
#else
  (void)window;
#endif
  }

void ImguiRenderer::shutdown() {
  if (vaoHandle) {
    glDeleteVertexArrays(1, &vaoHandle);
  }
  if (vboHandle) {
    glDeleteBuffers(1, &vboHandle);
  }
  if (elementsHandle) {
    glDeleteBuffers(1, &elementsHandle);
  }

  vaoHandle = vboHandle = elementsHandle = 0;

  if (shaderHandle && vertHandle) {
    glDetachShader(shaderHandle, vertHandle);
  }
  if (vertHandle) {
    glDeleteShader(vertHandle);
  }
  vertHandle = 0;

  if (shaderHandle && fragHandle) {
    glDetachShader(shaderHandle, fragHandle);
  }
  if (fragHandle) {
    glDeleteShader(fragHandle);
  }
  fragHandle = 0;

  if (shaderHandle) {
    glDeleteProgram(shaderHandle);
  }
  shaderHandle = 0;

  if (fontTexture) {
    glDeleteTextures(1, &fontTexture);
    ImGui::GetIO().Fonts->TexID = 0;
    fontTexture = 0;
  }
  ImGui::Shutdown();
}

bool ImguiRenderer::processEvent(SDL_Event* event) {
  ImGuiIO& io = ImGui::GetIO();

  switch (event->type) {
    case SDL_MOUSEWHEEL: {
      if (event->wheel.y > 0) mouseWheel = 1;
      if (event->wheel.y < 0) mouseWheel = -1;
      return true;
    }
    case SDL_MOUSEBUTTONDOWN: {
      if (event->button.button == SDL_BUTTON_LEFT) mousePressed[0] = true;
      if (event->button.button == SDL_BUTTON_RIGHT) mousePressed[1] = true;
      if (event->button.button == SDL_BUTTON_MIDDLE) mousePressed[2] = true;
      return true;
    }
    case SDL_TEXTINPUT: {
      io.AddInputCharactersUTF8(event->text.text);
      return true;
    }
    case SDL_KEYDOWN:
    case SDL_KEYUP: {
      int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
      io.KeysDown[key] = (event->type == SDL_KEYDOWN);
      io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
      io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
      io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
      io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
      return true;
    }
  }

  return false;
}

void ImguiRenderer::render() {
  ImGui::Begin("Another Window");
  static char buffer[256];
  ImGui::InputText("Terminal", buffer, sizeof(buffer));
  ImGui::End();

  ImGui::Render();
  renderDrawLists(ImGui::GetDrawData());
}

void ImguiRenderer::renderDrawLists(ImDrawData* draw_data) {
  // Avoid rendering when minimized, scale coordinates for retina displays
  // (screen coordinates != framebuffer coordinates)
  ImGuiIO& io = ImGui::GetIO();
  int fb_width =
      static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
  int fb_height =
      static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
  if (fb_width == 0 || fb_height == 0) {
    return;
  }
  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  // Backup GL state
  GLenum last_active_texture;
  glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
  glActiveTexture(GL_TEXTURE0);
  GLint last_program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  GLint last_sampler;
  glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
  GLint last_array_buffer;
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  GLint last_element_array_buffer;
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
  GLint last_vertex_array;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
  GLint last_polygon_mode[2];
  glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
  GLint last_viewport[4];
  glGetIntegerv(GL_VIEWPORT, last_viewport);
  GLint last_scissor_box[4];
  glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
  GLenum last_blend_src_rgb;
  glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
  GLenum last_blend_dst_rgb;
  glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
  GLenum last_blend_src_alpha;
  glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
  GLenum last_blend_dst_alpha;
  glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
  GLenum last_blend_equation_rgb;
  glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
  GLenum last_blend_equation_alpha;
  glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);

  GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
  GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
  GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
  GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

  // Setup render state: alpha-blending enabled, no face culling, no depth
  // testing, scissor enabled, polygon fill
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Setup viewport, orthographic projection matrix
  glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  const float ortho_projection[4][4] = {
      {2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f},
      {0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f},
      {0.0f, 0.0f, -1.0f, 0.0f},
      {-1.0f, 1.0f, 0.0f, 1.0f},
  };
  glUseProgram(shaderHandle);
  glUniform1i(attribLocationTex, 0);
  glUniformMatrix4fv(attribLocationProjMtx, 1, GL_FALSE,
                     &ortho_projection[0][0]);
  glBindVertexArray(vaoHandle);
  glBindSampler(0, 0);  // Rely on combined texture/sampler state.

  for (int n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    const ImDrawIdx* idx_buffer_offset = 0;

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
                 (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
                 (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
      if (pcmd->UserCallback) {
        pcmd->UserCallback(cmd_list, pcmd);
      } else {
        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
        glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w),
                  (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                  (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
        glDrawElements(
            GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
            sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
            idx_buffer_offset);
      }
      idx_buffer_offset += pcmd->ElemCount;
    }
  }

  // Restore modified GL state
  glUseProgram(last_program);
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindSampler(0, last_sampler);
  glActiveTexture(last_active_texture);
  glBindVertexArray(last_vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
  glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
  glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb,
                      last_blend_src_alpha, last_blend_dst_alpha);

  if (last_enable_blend) {
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }

  if (last_enable_cull_face) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }

  if (last_enable_depth_test) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }

  if (last_enable_scissor_test) {
    glEnable(GL_SCISSOR_TEST);
  } else {
    glDisable(GL_SCISSOR_TEST);
  }

  glPolygonMode(GL_FRONT_AND_BACK, last_polygon_mode[0]);

  glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2],
             (GLsizei)last_viewport[3]);

  glScissor(last_scissor_box[0], last_scissor_box[1],
            (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void ImguiRenderer::newFrame() {
  if (!fontTexture) {
    createDeviceObjects();
  }

  ImGuiIO& io = ImGui::GetIO();

  // Setup display size (every frame to accommodate for window resizing)
  int w, h;
  int display_w, display_h;
  SDL_GetWindowSize(window, &w, &h);
  SDL_GL_GetDrawableSize(window, &display_w, &display_h);
  io.DisplaySize = ImVec2((float)w, (float)h);
  io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0,
                                      h > 0 ? ((float)display_h / h) : 0);

  // Setup time step
  Uint32 time = SDL_GetTicks();
  double current_time = time / 1000.0;
  io.DeltaTime =
      (float)(current_time - mTime) > 0.0 ? (float)(current_time - mTime) : (float)(1.0f / 60.0f);
  mTime = current_time;

  // Setup inputs
  // (we already got mouse wheel, keyboard keys & characters from
  // SDL_PollEvent())
  int mx, my;
  Uint32 mouseMask = SDL_GetMouseState(&mx, &my);
  if (SDL_GetWindowFlags(window) & SDL_WINDOW_MOUSE_FOCUS) {
    io.MousePos = ImVec2((float)mx, (float)my);  // Mouse position, in pixels
                                                 // (set to -1,-1 if no mouse
                                                 // / on another screen, etc.)
  } else {
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
  }

  // If a mouse press event came, always pass
  // it as "mouse held this frame", so we
  // don't miss click-release events that are
  // shorter than 1 frame.
  io.MouseDown[0] =
      mousePressed[0] || (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
  io.MouseDown[1] =
      mousePressed[1] || (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
  io.MouseDown[2] =
      mousePressed[2] || (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
  mousePressed[0] = mousePressed[1] = mousePressed[2] = false;

  io.MouseWheel = mouseWheel;
  mouseWheel = 0.0f;

  // Hide OS mouse cursor if ImGui is drawing it
  SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

  // Start the frame. This call will update the io.WantCaptureMouse,
  // io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not)
  // to your application.
  ImGui::NewFrame();
}

bool ImguiRenderer::isFocus() const {
  return ImGui::IsMouseHoveringAnyWindow();
}

bool ImguiRenderer::createDeviceObjects() {
  // Backup GL state
  GLint last_texture, last_array_buffer, last_vertex_array;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

  const GLchar* vertex_shader =
      "#version 330\n"
      "uniform mat4 ProjMtx;\n"
      "in vec2 Position;\n"
      "in vec2 UV;\n"
      "in vec4 Color;\n"
      "out vec2 Frag_UV;\n"
      "out vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "	Frag_UV = UV;\n"
      "	Frag_Color = Color;\n"
      "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
      "}\n";

  const GLchar* fragment_shader =
      "#version 330\n"
      "uniform sampler2D Texture;\n"
      "in vec2 Frag_UV;\n"
      "in vec4 Frag_Color;\n"
      "out vec4 Out_Color;\n"
      "void main()\n"
      "{\n"
      "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
      "}\n";

  shaderHandle = glCreateProgram();
  vertHandle = glCreateShader(GL_VERTEX_SHADER);
  fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertHandle, 1, &vertex_shader, 0);
  glShaderSource(fragHandle, 1, &fragment_shader, 0);
  glCompileShader(vertHandle);
  glCompileShader(fragHandle);
  glAttachShader(shaderHandle, vertHandle);
  glAttachShader(shaderHandle, fragHandle);
  glLinkProgram(shaderHandle);

  attribLocationTex = glGetUniformLocation(shaderHandle, "Texture");
  attribLocationProjMtx = glGetUniformLocation(shaderHandle, "ProjMtx");
  attribLocationPosition = glGetAttribLocation(shaderHandle, "Position");
  attribLocationUV = glGetAttribLocation(shaderHandle, "UV");
  attribLocationColor = glGetAttribLocation(shaderHandle, "Color");

  glGenBuffers(1, &vboHandle);
  glGenBuffers(1, &elementsHandle);

  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  glEnableVertexAttribArray(attribLocationPosition);
  glEnableVertexAttribArray(attribLocationUV);
  glEnableVertexAttribArray(attribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t) & (((TYPE*)0)->ELEMENT))
  glVertexAttribPointer(attribLocationPosition, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(attribLocationUV, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

  createFontsTexture();

  // Restore modified GL state
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindVertexArray(last_vertex_array);

  return true;
}

void ImguiRenderer::createFontsTexture() {
  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(
      &pixels, &width, &height);  // Load as RGBA 32-bits for OpenGL3 demo
                                  // because it is more likely to be
                                  // compatible with user's existing shader.

  // Upload texture to graphics system
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGenTextures(1, &fontTexture);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels);

  // Store our identifier
  io.Fonts->TexID = (void*)(intptr_t)fontTexture;

  // Restore state
  glBindTexture(GL_TEXTURE_2D, last_texture);
}

}  // namespace radix
