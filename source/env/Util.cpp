#include <radix/env/Util.hpp>

#include <thread>

#include <radix/env/Environment.hpp>
#include <radix/env/OperatingSystem.hpp>
#include <radix/core/diag/AnsiConsoleLogger.hpp>
#include <radix/core/diag/StdoutLogger.hpp>

#ifdef _WIN32
  #include <windows.h>
  const DWORD MS_VC_EXCEPTION=0x406D1388;
  #pragma pack(push,8)
  typedef struct tagTHREADNAME_INFO
  {
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
  } THREADNAME_INFO;
  #pragma pack(pop)
#elif __linux__
  #include <sys/prctl.h>
#endif

namespace radix {

std::unique_ptr<Logger> Util::logger;
Util::_Log Util::Log;

std::random_device Util::RandDev;
std::mt19937 Util::Rand(Util::RandDev());

void Util::Init() {
  if (OperatingSystem::IsLinux()) {
    logger.reset(new AnsiConsoleLogger);
  } else {
    logger.reset(new StdoutLogger);
  }
}

LogInput Util::_Log::operator()() {
  LogLevel level = Environment::getConfig().safeGetlogLevel();
  Environment::getConfig().relinquishlogLevelMutex();
  return LogInput(*Util::logger, level);
}

LogInput Util::_Log::operator()(LogLevel lvl) {
  return LogInput(*Util::logger, lvl);
}

LogInput Util::_Log::operator()(LogLevel lvl, const std::string &tag) {
  return LogInput(*Util::logger, lvl, tag);
}

#ifdef _WIN32
static void WinSetThreadName(uint32_t dwThreadID, const char **threadName) {
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = *threadName;
  info.dwThreadID = dwThreadID;
  info.dwFlags = 0;
  __try
  {
    RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
  }
}

void Util::SetThreadName(std::thread &thread, const char *name) {
  DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread.native_handle()));
  WinSetThreadName(threadId, &name);
}
#elif __APPLE__
void Util::SetThreadName(std::thread &thread, const char *name) {
  pthread_setname_np(name);
}
#else
void Util::SetThreadName(std::thread &thread, const char *name) {
  auto handle = thread.native_handle();
  pthread_setname_np(handle, name);
}
#endif

} /* namespace radix */
