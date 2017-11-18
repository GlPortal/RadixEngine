#include <radix/util/KeyDecoder.hpp>

#include <SDL_scancode.h>

namespace radix {
  
const std::map<std::string, int> KeyDecoder::lookupTable = { 
  {"escape", SDL_SCANCODE_ESCAPE},
  {"return", SDL_SCANCODE_RETURN},
  {"space", SDL_SCANCODE_SPACE},
  {"up", SDL_SCANCODE_UP},
  {"down", SDL_SCANCODE_DOWN},
  {"left", SDL_SCANCODE_LEFT},
  {"right", SDL_SCANCODE_RIGHT},
};

int KeyDecoder::decodeString(const std::string &keyString) {
  return lookupTable.find(keyString)->second;
}

} /* namespace radix */
