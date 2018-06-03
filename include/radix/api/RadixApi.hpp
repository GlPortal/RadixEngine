#ifndef RADIX_ENGINE_API_HPP
#define RADIX_ENGINE_API_HPP

#include <string>

#include <angelscript.h>

#include <radix/World.hpp>

namespace radix {

class RadixApi {
private:
  World &world;
public:
  RadixApi(World &world);
  void registerFunctions(asIScriptEngine *asEngine);
  void exit();
  void logDebug(std::string category, std::string message);
};

} /* namespace radix */

#endif /* RADIX_ENGINE_API_HPP */
