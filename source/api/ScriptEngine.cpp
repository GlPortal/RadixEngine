#include <radix/api/ScriptEngine.hpp>

#include <string>
#include <cstring>

namespace radix {

  ScriptEngine::ScriptEngine(World &world): world(world), playerApi(world){
  playerApi.registerFunctions(chaiScriptEngine);
}

void ScriptEngine::runCode(std::string code) {
  chaiScriptEngine.eval(code);
}

} /* namespace radix */

