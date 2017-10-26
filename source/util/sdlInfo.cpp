#include "radix/util/sdlInfo.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_cpuinfo.h>
#include <SDL2/SDL_syswm.h>
#include <GL/gl.h>

using map = SdlInfo::map;

bool SdlInfo::isinitialized = false;

bool SdlInfo::init_SDL() {
  if (isinitialized) return true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Can not Create SDL init\n";
    return false;
  }

  return (isinitialized = true);
}

bool SdlInfo::destory_SDL() {
  if (!isinitialized) return false;

  SDL_Quit();
  return (isinitialized = false);
}

bool SdlInfo::read_SDL(map& sdlVersion, map& displayServer, map& glVersionInfo,
                       map& gpu, map& cpuInfo, map& memInfo) {
  if (!init_SDL()) return false;

  SDL_SysWMinfo sysinfo = {};
  // Create SDL window with OpenGL and Hidden
  auto window =
      SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
  if (!window) {
    destory_SDL();
    return false;
  }
  // Create OpenGL Context
  auto maincontext = SDL_GL_CreateContext(window);
  // Get SDL Version
  SDL_GetVersion(&sysinfo.version);
  std::string version = std::to_string(sysinfo.version.major) + "." +
                        std::to_string(sysinfo.version.minor);
  sdlVersion["version"].swap(version);

  SDL_GetWindowWMInfo(window, &sysinfo);
  switch (sysinfo.subsystem) {
    case SDL_SYSWM_UNKNOWN:
      break;
    case SDL_SYSWM_WINDOWS:
      displayServer["subsystem"] = "Microsoft Windows(TM)";
      break;
    case SDL_SYSWM_X11:
      displayServer["subsystem"] = "X Window System";
      break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
    case SDL_SYSWM_WINRT:
      displayServer["subsystem"] = "WinRT";
      break;
#endif
    case SDL_SYSWM_DIRECTFB:
      displayServer["subsystem"] = "DirectFB";
      break;
    case SDL_SYSWM_COCOA:
      displayServer["subsystem"] = "Apple OS X";
      break;
    case SDL_SYSWM_UIKIT:
      displayServer["subsystem"] = "UIKit";
      break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
    case SDL_SYSWM_WAYLAND:
      displayServer["subsystem"] = "Wayland";
      break;
    case SDL_SYSWM_MIR:
      displayServer["subsystem"] = "Mir";
      break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
    case SDL_SYSWM_ANDROID:
      displayServer["subsystem"] = "Android";
      break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
    case SDL_SYSWM_VIVANTE:
      displayServer["subsystem"] = "Vivante";
      break;
#endif
  }
  // Get GPU Vendor
  std::string glVendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
  gpu["glVendor"].swap(glVendor);
  // Get OpenGL Version
  std::string glVersion =
      reinterpret_cast<char const*>(glGetString(GL_VERSION));
  glVersionInfo["glVersion"].swap(glVersion);
  // Get Renderer
  std::string glRenderer =
      reinterpret_cast<char const*>(glGetString(GL_RENDERER));
  if (glRenderer.find("NVIDIA") != std::string::npos) {
    const GLenum GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX = 0x9048;
    const GLenum GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX = 0x9049;
    GLint nTotalMemoryInKB = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &nTotalMemoryInKB);
    std::string TotalMemory = std::to_string(nTotalMemoryInKB) + "KB";
    glVersionInfo["TotalMemory"].swap(TotalMemory);

    GLint nCurAvailMemoryInKB = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
                  &nCurAvailMemoryInKB);
    std::string currentAvailableMemory =
        std::to_string(nCurAvailMemoryInKB) + "KB";
    glVersionInfo["CurrentMemory"].swap(currentAvailableMemory);
  } else if (glRenderer.find("AMD") || glRenderer.find("ATI")) {
  } else if (glRenderer.find("Intel")) {
  }
  gpu["glRenderer"].swap(glRenderer);

  cpuInfo["hyperThreadsCount"] = SDL_GetCPUCount();
  cpuInfo["cacheSize"] = SDL_GetCPUCacheLineSize();
  memInfo["MemTotal"] = SDL_GetSystemRAM();

  // CleanUp
  SDL_GL_DeleteContext(maincontext);
  SDL_DestroyWindow(window);
  return true;
}
