#include <radix/api/PlayerApi.hpp>
#include <radix/entities/Player.hpp>
namespace radix {

  PlayerApi::PlayerApi(World &world): world(world) {
  }

  void PlayerApi::registerFunctions(chaiscript::ChaiScript &scriptEngine) {
    scriptEngine.add(chaiscript::fun(&PlayerApi::kill, this), "killPlayer");
    scriptEngine.add(chaiscript::fun(&PlayerApi::moveY, this), "playerMoveY");
  }

  void PlayerApi::kill() {
    world.getPlayer().kill();
  }

  void PlayerApi::moveY(float distance) {
    world.getPlayer().moveY(distance);
  }
} /* namespace radix */

