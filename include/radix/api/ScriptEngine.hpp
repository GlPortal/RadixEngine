#ifndef RADIX_SCRIPT_ENGINE_HPP
#define RADIX_SCRIPT_ENGINE_HPP

#include <string>
#include <radix/World.hpp>
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <radix/api/PlayerApi.hpp>
#include <radix/api/RadixApi.hpp>

namespace radix {

class ScriptEngine {
private:
  World &world;
  PlayerApi playerApi;
  chaiscript::ChaiScript chaiScriptEngine;
public:
  ScriptEngine(World &world);
  void runCode(std::string code);
};

} /* namespace radix */

#endif /* RADIX_SCRIPT_ENGINE_HPP */
