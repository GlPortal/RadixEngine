#ifndef RADIX_SCRIPT_ENGINE_HPP
#define RADIX_SCRIPT_ENGINE_HPP

#include <string>

#include <angelscript.h>

#include <radix/World.hpp>
#include <radix/api/PlayerApi.hpp>
#include <radix/api/RadixApi.hpp>

namespace radix {

class ScriptEngine {
private:
  World &world;
  PlayerApi playerApi;
  RadixApi radixApi;
  asIScriptEngine *angelScript;

public:
  ScriptEngine(World &world);
  ~ScriptEngine();
  void runCode(const std::string &code);
};

} /* namespace radix */

#endif /* RADIX_SCRIPT_ENGINE_HPP */
