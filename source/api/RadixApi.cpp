#include <radix/api/RadixApi.hpp>

namespace radix {

  RadixApi::PlayerApi(World &world): world(world) {
  }

  void RadixApi::registerFunctions(chaiscript::ChaiScript &scriptEngine) {
    scriptEngine.add(chaiscript::fun(&RadixApi::exit, this), "exit");
  }

  void RadixApi::exit() {
    exit;
  }
} /* namespace radix */
