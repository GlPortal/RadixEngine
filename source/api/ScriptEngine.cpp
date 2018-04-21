#include <radix/api/ScriptEngine.hpp>

#include <string>
#include <cstring>

namespace radix {

ScriptEngine::ScriptEngine(World &world): world(world){
  chai.add(chaiscript::fun(&helloWorld), "helloWorld");

  chai.eval("puts(helloWorld(\"Bob\"));");
}

void ScriptEngine::runCode(std::string string) {
}

} /* namespace radix */

