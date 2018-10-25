#include <radix/env/Util.hpp>

#include <thread>

#include <radix/env/LegacyEnvironment.hpp>
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
#elif __APPLE__
  #include <cstring> // std::strlen
  #include <system_error>
#elif __linux__
  #include <cstring> // std::strlen
  #include <sys/prctl.h> // pthread_*
  #include <system_error>
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

void Util::SetCurrentThreadName(const char *name) {
  DWORD threadId = ::GetThreadId((HANDLE)(GetCurrentThreadId()));
  WinSetThreadName(threadId, &name);
}
#elif __APPLE__
void Util::SetCurrentThreadName(const char *name) {
  if (std::strlen(name) > 15) {
    throw std::length_error("Name exceeds lentgh of 15 characters");
  }
  int ret = pthread_setname_np(name);
  if (ret != 0) {
    throw std::system_error(ret, std::system_category(),
        std::string("Setting thread name to \"") + name + '"');
  }
}
#else
void Util::SetCurrentThreadName(const char *name) {
  if (std::strlen(name) > 15) {
    throw std::length_error("Name exceeds lentgh of 15 characters");
  }
  int ret = pthread_setname_np(pthread_self(), name);
  if (ret != 0) {
    throw std::system_error(ret, std::system_category(),
        std::string("Setting thread name to \"") + name + '"');
  }
}
#endif

} /* namespace radix */
