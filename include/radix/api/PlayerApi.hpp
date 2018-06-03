#ifndef RADIX_PLAYER_API_HPP
#define RADIX_PLAYER_API_HPP

#include <angelscript.h>

#include <radix/World.hpp>

namespace radix {

class PlayerApi {
private:
  World &world;
public:
  PlayerApi(World &world);
  void registerFunctions(asIScriptEngine *asEngine);
  void kill();
  void moveY(float distance);
  void moveX(float distance);
  void jump();
};

} /* namespace radix */

#endif /* RADIX_PLAYER_API_HPP */
