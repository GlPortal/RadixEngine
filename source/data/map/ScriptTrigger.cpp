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
#include <radix/api/ScriptEngine.hpp>

using namespace std;

namespace radix {

const std::string ScriptTrigger::TYPE = "script";

ScriptTrigger::ScriptTrigger(const std::string &code) :
  code(code) {
}

void ScriptTrigger::addAction(Entity &entity) {
  entities::Trigger &trigger = dynamic_cast<entities::Trigger&>(entity);
  std::string sourceCode = code;
  trigger.setActionOnEnter([sourceCode] (entities::Trigger &trigger) {
      BaseGame &game = trigger.world.game;
      ScriptEngine* scriptEngine = game.getScriptEngine();
      scriptEngine->runCode(sourceCode);
  });
}

} /* namespace radix */
