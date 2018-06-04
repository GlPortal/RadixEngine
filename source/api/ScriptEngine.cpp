#include <radix/api/ScriptEngine.hpp>

#include <stdexcept>

#include <scriptstdstring/scriptstdstring.h>
#include <scripthandle/scripthandle.h>
#include <weakref/weakref.h>

#include <radix/env/Util.hpp>

namespace radix {

void MessageCallback(const asSMessageInfo *message, void *parameter) {
  LogLevel level;
  switch (message->type) {
  case asMSGTYPE_INFORMATION: level = LogLevel::Info; break;
  case asMSGTYPE_WARNING: level = LogLevel::Warning; break;
  case asMSGTYPE_ERROR: level = LogLevel::Error; break;
  }
  Util::Log(level, "AngelScript") << message->section << " (" << message->row << ", " << message->col << "): "
      << message->message;
}

ScriptEngine::ScriptEngine(World &world): world(world), playerApi(world), radixApi(world) {
  angelScript = asCreateScriptEngine();
  if (angelScript == nullptr) {
    throw std::runtime_error("Failed to create AngelScript engine");
  }
  angelScript->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
  RegisterStdString(angelScript);
  RegisterScriptHandle(angelScript);
  RegisterScriptWeakRef(angelScript);

  playerApi.registerFunctions(angelScript);
  radixApi.registerFunctions(angelScript);
}

ScriptEngine::~ScriptEngine() {
  angelScript->ShutDownAndRelease();
}

void ScriptEngine::runCode(const std::string &code) {
  std::string actualcode = "void main() {\n" + code + "\n}";
  asIScriptModule *module = angelScript->GetModule(0, asGM_ALWAYS_CREATE);
  int result = module->AddScriptSection("script", &actualcode[0], actualcode.size());
  if (result < 0) {
    throw std::runtime_error("AddScriptSection() failed");
  }
  result = module->Build();
  if (result < 0) {
    throw std::runtime_error("Build() failed");
  }
  asIScriptContext *context = angelScript->CreateContext();
  if (context == nullptr) {
    throw std::runtime_error("Failed to create AngelScript context");
  }
  asIScriptFunction *function = angelScript->GetModule(0)->GetFunctionByDecl("void main()");
  if (function == nullptr) {
    throw std::runtime_error("No main() function");
  }
  result = context->Prepare(function);
  if (result < 0) {
    throw std::runtime_error("Failed to prepare context");
  }
  result = context->Execute();
  context->Release();
}

} /* namespace radix */

