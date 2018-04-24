#ifndef RADIX_ENGINE_API_HPP
#define RADIX_ENGINE_API_HPP
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <radix/World.hpp>

namespace radix {
  class RadixApi {
  private:
    World &world;
  public:
    RadixApi(World &world);
    void exit();
  };

} /* namespace radix */

#endif /* RADIX_ENGINE_API_HPP */
