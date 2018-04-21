#ifndef RADIX_SCRIPT_ENGINE_HPP
#define RADIX_SCRIPT_ENGINE_HPP

#include <string>
#include <radix/World.hpp>
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

namespace radix {

class ScriptEngine {
private:
  World &world;
  chaiscript::ChaiScript chaiScriptEngine;
public:
  ScriptEngine(World &world);
  void run(std::string string);
};

} /* namespace radix */

#endif /* RADIX_SCRIPT_ENGINE_HPP */
