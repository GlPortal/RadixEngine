#include <radix/api/PlayerApi.hpp>
#include <radix/entities/Player.hpp>
namespace radix {

  PlayerApi::PlayerApi(World &world): world(world) {
  }

  void PlayerApi::registerFunctions(chaiscript::ChaiScript &scriptEngine) {
    scriptEngine.add(chaiscript::fun(&PlayerApi::kill, this), "killPlayer");
    scriptEngine.add(chaiscript::fun(&PlayerApi::moveY, this), "playerMoveY");
    scriptEngine.add(chaiscript::fun(&PlayerApi::moveX, this), "playerMoveX");
    scriptEngine.add(chaiscript::fun(&PlayerApi::jump, this), "playerJump");
  }

  void PlayerApi::kill() {
    world.getPlayer().kill();
  }

  void PlayerApi::moveY(float distance) {
    world.getPlayer().moveY(distance);
  }

  void PlayerApi::moveX(float distance) {
    world.getPlayer().moveX(distance);
  }

  void PlayerApi::jump() {
    world.getPlayer().jump();
  }
} /* namespace radix */

