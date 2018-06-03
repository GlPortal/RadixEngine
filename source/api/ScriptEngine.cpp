#include <radix/api/ScriptEngine.hpp>

#include <stdexcept>

#include <scriptstdstring/scriptstdstring.h>
#include <scripthandle/scripthandle.h>
#include <weakref/weakref.h>

#include <radix/env/Util.hpp>

namespace radix {

void MessageCallback(const asSMessageInfo *msg, void *param) {
  LogLevel lvl;
  switch (msg->type) {
  case asMSGTYPE_INFORMATION: lvl = LogLevel::Info; break;
  case asMSGTYPE_WARNING: lvl = LogLevel::Warning; break;
  case asMSGTYPE_ERROR: lvl = LogLevel::Error; break;
  }
  Util::Log(lvl, "AngelScript") << msg->section << " (" << msg->row << ", " << msg->col << "): "
      << msg->message;
}

ScriptEngine::ScriptEngine(World &world): world(world), playerApi(world), radixApi(world) {
  asEngine = asCreateScriptEngine();
  if (asEngine == nullptr) {
    throw std::runtime_error("Failed to create AngelScript engine");
  }
  asEngine->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
  RegisterStdString(asEngine);
  RegisterScriptHandle(asEngine);
  RegisterScriptWeakRef(asEngine);

  playerApi.registerFunctions(asEngine);
  radixApi.registerFunctions(asEngine);
}

ScriptEngine::~ScriptEngine() {
  asEngine->ShutDownAndRelease();
}

void ScriptEngine::messageCallback(const asSMessageInfo *msg, void *param) {
  LogLevel lvl;
  switch (msg->type) {
  case asMSGTYPE_INFORMATION: lvl = LogLevel::Info; break;
  case asMSGTYPE_WARNING: lvl = LogLevel::Warning; break;
  case asMSGTYPE_ERROR: lvl = LogLevel::Error; break;
  }
  Util::Log(lvl, "AngelScript") << msg->section << " (" << msg->row << ", " << msg->col << "): "
      << msg->message;
}

void ScriptEngine::runCode(const std::string &code) {
  std::string actualcode = "void main() {\n" + code + "\n}";
  asIScriptModule *mod = asEngine->GetModule(0, asGM_ALWAYS_CREATE);
  int r = mod->AddScriptSection("script", &actualcode[0], actualcode.size());
  if (r < 0) {
    throw std::runtime_error("AddScriptSection() failed");
  }
  r = mod->Build();
  if (r < 0) {
    throw std::runtime_error("Build() failed");
  }
  asIScriptContext *ctx = asEngine->CreateContext();
  if (ctx == nullptr) {
    throw std::runtime_error("Failed to create AngelScript context");
  }
  asIScriptFunction *func = asEngine->GetModule(0)->GetFunctionByDecl("void main()");
  if (func == nullptr) {
    throw std::runtime_error("No main() function");
  }
  r = ctx->Prepare(func);
  if (r < 0) {
    throw std::runtime_error("Failed to prepare context");
  }
  r = ctx->Execute();
  ctx->Release();
}

} /* namespace radix */

