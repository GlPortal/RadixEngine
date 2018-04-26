#include <radix/api/RadixApi.hpp>
#include <radix/env/Util.hpp>

namespace radix {

RadixApi::RadixApi(World &world): world(world) {
}

void RadixApi::registerFunctions(chaiscript::ChaiScript &scriptEngine) {
  scriptEngine.add(chaiscript::fun(&RadixApi::exit, this), "exit");
  scriptEngine.add(chaiscript::fun(&RadixApi::logDebug, this), "logDebug");
}

void RadixApi::exit() {
}

void RadixApi::logDebug(std::string category, std::string message) {
    //    Util::Log(LogLevel::Debug, category) << message;
}

} /* namespace radix */
