#include <radix/api/RadixApi.hpp>

#include <radix/core/diag/Logger.hpp>
#include <radix/env/Util.hpp>

namespace radix {

RadixApi::RadixApi(World &world): world(world) {
}

void RadixApi::registerFunctions(asIScriptEngine *asEngine) {
  asEngine->RegisterGlobalFunction("void exit()", asMETHOD(RadixApi, exit), asCALL_THISCALL_ASGLOBAL, this);
  asEngine->RegisterGlobalFunction("void logDebug(const string &in category, const string &in message)", asMETHOD(RadixApi, logDebug), asCALL_THISCALL_ASGLOBAL, this);
}

void RadixApi::exit() {
}

void RadixApi::logDebug(std::string category, std::string message) {
  Util::Log(LogLevel::Debug, category) << message;
}

} /* namespace radix */
