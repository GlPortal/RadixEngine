#ifndef DEBUG_INFO_HPP
#define DEBUG_INFO_HPP
#include <string>
#include <unordered_map>
#include <array>

class DebugInfo {
  public:
  enum SystemInfoKey {
    Kernel = 0,
    Distro,
    CPU,
    Mem,
    GLibC,
    DisplayServer,
    SDL_Version,
    GPU,
    OpenGL,
    COUNT
  };

  static std::string getKernel_Name();

  static std::string getKernel_Arch();

  static std::string getKernel_Release();

  static std::string getCPU_Name();

  static std::string getCPU_Vendor();

  static std::string getCPU_Cache();

  static std::string getCPU_Cores();

  static std::string getCPU_hyperThreadingCount();

  static std::string getMem_Total();

  static std::string getMem_Free(bool forceToReload = false);

  static std::string getOS_Name();

  static std::string getOS_Version();

  static std::string getLibC_Version();

  static std::string getDisplayServer();

  static std::string getSDL_Version();

  static std::string getGPU_Name();

  static std::string getGPU_Vendor();

  static std::string getGPU_Size();

  static std::string getOpenGL_Version();

  static bool isInitlized();

  using stringMap = std::unordered_map<std::string, std::string>;
  static std::array<stringMap, SystemInfoKey::COUNT> mInfo;
};

#endif  //! DEBUG_INFO_HPP
