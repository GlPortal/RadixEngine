#include <radix/data/map/ScriptTrigger.hpp>
#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/core/state/GameStateManager.hpp>
#include <radix/env/Environment.hpp>
#include <radix/entities/traits/HealthTrait.hpp>
#include <radix/World.hpp>
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <radix/api/PlayerApi.hpp>

using namespace std;

namespace radix {

const std::string ScriptTrigger::TYPE = "script";

ScriptTrigger::ScriptTrigger(const std::string &code) :
  code(code) {
}

void ScriptTrigger::addAction(Entity &entity) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(entity);
  BaseGame &game = trigger.world.game;
  chaiscript::ChaiScript& scriptEngine = game.getScriptEngine();
  PlayerApi playerApi(trigger.world);
  playerApi.registerFunctions(scriptEngine);
  std::string sourceCode = code;
  trigger.setActionOnEnter([sourceCode] (entities::Trigger &trigger) {
      BaseGame &game = trigger.world.game;
      //      chaiscript::ChaiScript& scriptEngine = game.getScriptEngine();
      //      scriptEngine.eval(sourceCode);
  });
}

} /* namespace radix */
