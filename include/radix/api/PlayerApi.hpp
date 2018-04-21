#ifndef RADIX_PLAYER_API_HPP
#define RADIX_PLAYER_API_HPP
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <radix/World.hpp>

namespace radix {
  class PlayerApi {
  private:
    World &world;
  public:
    PlayerApi(World &world);
    void registerFunctions(chaiscript::ChaiScript &scriptEngine);
    void kill();
    void moveY(float distance);
  };

} /* namespace radix */

#endif /* RADIX_PLAYER_API_HPP */
