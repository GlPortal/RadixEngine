#include <radix/api/ScriptEngine.hpp>

#include <string>
#include <cstring>

namespace radix {

ScriptEngine::ScriptEngine(World &world): world(world), playerApi(world), radixApi(world) {
  playerApi.registerFunctions(chaiScriptEngine);
  radixApi.registerFunctions(chaiScriptEngine);
}

void ScriptEngine::runCode(std::string code) {
  chaiScriptEngine.eval(code);
}

} /* namespace radix */

