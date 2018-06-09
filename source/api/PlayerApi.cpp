#include <radix/api/PlayerApi.hpp>

#include <radix/entities/Player.hpp>

namespace radix {

PlayerApi::PlayerApi(World &world): world(world) {
}

void PlayerApi::registerFunctions(asIScriptEngine *angelScript) {
  angelScript->RegisterGlobalFunction("void killPlayer()", asMETHOD(PlayerApi, kill), asCALL_THISCALL_ASGLOBAL, this);
  angelScript->RegisterGlobalFunction("void playerMoveY(float distance)", asMETHOD(PlayerApi, moveY), asCALL_THISCALL_ASGLOBAL, this);
  angelScript->RegisterGlobalFunction("void playerMoveX(float distance)", asMETHOD(PlayerApi, moveX), asCALL_THISCALL_ASGLOBAL, this);
  angelScript->RegisterGlobalFunction("void playerJump()", asMETHOD(PlayerApi, jump), asCALL_THISCALL_ASGLOBAL, this);
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

