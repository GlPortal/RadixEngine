#include <radix/env/OperatingSystem.hpp>
#include <string>
#include <SDL2/SDL_platform.h>

namespace radix {

bool OperatingSystem::IsLinux() {
  if (std::string("Linux") == SDL_GetPlatform()) {
    return true;
  }
  return false;
}

} /* namespace radix */
