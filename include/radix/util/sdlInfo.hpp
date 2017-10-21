#ifndef SDL_INFO_HPP
#define SDL_INFO_HPP
#include <unordered_map>
#include <string>

struct SdlInfo {
  using map = std::unordered_map<std::string, std::string>;

  static bool init_SDL();
  static bool read_SDL(map& sdlVersion, map& displayServer, map& glVersionInfo,
                       map& gpu, map& cpuInfo, map& memInfo);
  static bool destory_SDL();

  static bool isinitialized;
};
#endif  //! SDL_INFO_HPP
