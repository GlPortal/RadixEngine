#ifndef RADIX_ENGINE_API_HPP
#define RADIX_ENGINE_API_HPP
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <radix/World.hpp>
#include <string>

namespace radix {

class RadixApi {
private:
  World &world;
public:
  RadixApi(World &world);
  void
  registerFunctions(chaiscript::ChaiScript &scriptEngine);
  void exit();
  void logDebug(std::string category, std::string message);
};

} /* namespace radix */

#endif /* RADIX_ENGINE_API_HPP */
