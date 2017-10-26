#include "windows/systemInfo.hpp"
#include <iostream>
#include <array>
#include <vector>

#ifdef _MSC_VER
typedef BOOL(WINAPI* LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
#endif

using map = systemInfo::map;

std::vector<std::string> systemInfo::readRegsitery(
    const std::string& regKey, const std::vector<std::string>& keys) {
  DWORD dwMHz = _MAX_PATH;
  HKEY hKey;

  // open the key where the proc speed is hidden:
  long lError =
      RegOpenKeyEx(HKEY_LOCAL_MACHINE, regKey.c_str(), 0, KEY_READ, &hKey);
  if (lError != ERROR_SUCCESS) {
    std::cerr << "Can not found " << regKey << '\n';
    return {};
  }

  DWORD bufferSize = 256;
  char buffer[256];
  std::vector<std::string> output(keys.size());
  for (int i = 0; i < output.size(); ++i) {
    RegQueryValueEx(hKey, keys[i].c_str(), NULL, NULL, (LPBYTE)buffer,
                    &bufferSize);
    output[i] = buffer;
    memset(buffer, 0, sizeof(buffer));
  }
  return output;
}

void systemInfo::readCPUInfo(map& cpuInfo) {
  const std::string cpuRegKey =
      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
  const std::string cpuNameValueData = "ProcessorNameString";
  const std::string vendorNameValueData = "VendorIdentifier";
  // query the key:
  auto info = readRegsitery(cpuRegKey, {cpuNameValueData, vendorNameValueData});
  cpuInfo["modelName"] = info[0];
  cpuInfo["vendorId"] = info[1];
}

// Helper function to count set bits in the processor mask.
DWORD systemInfo::CountSetBits(ULONG_PTR bitMask) {
  DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
  DWORD bitSetCount = 0;
  ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
  DWORD i;

  for (i = 0; i <= LSHIFT; ++i) {
    bitSetCount += ((bitMask & bitTest) ? 1 : 0);
    bitTest /= 2;
  }

  return bitSetCount;
}

void systemInfo::readSystemInfo(map& kernelInfo, map& cpuInfo, map& memInfo) {
  {  // read
     // https://msdn.microsoft.com/en-us/library/windows/desktop/ms683194(v=vs.85).aspx
     // Read CPU Count cores and Cache
    DWORD byteOffset = 0;
    DWORD returnLength = 0;
    BOOL done = FALSE;

    readCPUInfo(cpuInfo);

#ifdef _MSC_VER
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;

    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;

    DWORD processorL1Cache = 0;
    DWORD processorL2Cache = 0;
    DWORD processorL3Cache = 0;

    DWORD processorPackageCount = 0;
    PCACHE_DESCRIPTOR Cache;

    LPFN_GLPI glpi = (LPFN_GLPI)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")), "GetLogicalProcessorInformation");

    while (!done) {
      DWORD rc = glpi(buffer, &returnLength);

      if (FALSE == rc) {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
          if (buffer) free(buffer);

          buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);

          if (NULL == buffer) break;
        } else {
          break;
        }
      } else {
        done = TRUE;
      }
    }

    ptr = buffer;

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <=
           returnLength) {
      switch (ptr->Relationship) {
        case RelationNumaNode:
          // Non-NUMA systems report a single record of this type.
          numaNodeCount++;
          break;

        case RelationProcessorCore:
          processorCoreCount++;

          // A hyperthreaded core supplies more than one logical processor.
          logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
          break;

        case RelationCache:
          // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for
          // each cache.
          Cache = &ptr->Cache;
          if (Cache->Level == 1) {
            processorL1CacheCount++;
            processorL1Cache = Cache->Size;
          } else if (Cache->Level == 2) {
            processorL2CacheCount++;
            processorL2Cache = Cache->Size;
          } else if (Cache->Level == 3) {
            processorL3CacheCount++;
            processorL3Cache = Cache->Size;
          }
          break;

        case RelationProcessorPackage:
          // Logical processors share a physical package.
          processorPackageCount++;
          break;

        default:
          break;
      }
      byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
      ptr++;
    }

    cpuInfo["cacheSize"] = std::to_string(int(processorL1Cache / 1024)) + "kB";
    cpuInfo["cacheSize2"] = std::to_string(int(processorL2Cache / 1024)) + "kB";
    cpuInfo["cacheSize3"] = std::to_string(int(processorL3Cache / 1024)) + "kB";
    // cpuInfo["cpuCores"] = std::to_string(processorCoreCount);
    // std::cout << "processorCoreCount : " << processorCoreCount << '/n';
    cpuInfo["hyperThreadsCount"] = std::to_string(logicalProcessorCount);
#endif
  }

  SYSTEM_INFO info;
  GetSystemInfo(&info);
  // read
  // https://msdn.microsoft.com/en-us/library/windows/desktop/ms724958(v=vs.85).aspx
  switch (info.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
      kernelInfo["machine"] = "x64";
      break;
    case PROCESSOR_ARCHITECTURE_INTEL:
      kernelInfo["machine"] = "x86";
      break;
    case PROCESSOR_ARCHITECTURE_ARM:
      kernelInfo["machine"] = "ARM";
      break;
    default:
      kernelInfo["machine"] = "UnKnown";
      break;
  }
  cpuInfo["cpuCores"] = std::to_string(info.dwNumberOfProcessors);

  {  // Read Memory
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    memInfo["MemTotal"] =
        std::to_string((statex.ullTotalPhys / 1024) / 1024) + "MB";

    memInfo["SwapTotal"] =
        std::to_string((statex.ullTotalPageFile / 1024) / 1024) + "MB";

    memInfo["MemFree"] =
        std::to_string((statex.ullAvailPhys / 1024) / 1024) + "MB";
  }
}

std::string systemInfo::getWindowsName() {
  OSVERSIONINFO osvi;
  BOOL bIsWindowsXPorLater;

  ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  GetVersionEx(&osvi);

  std::string windowsVersion;
  switch (osvi.dwMajorVersion) {
    case 10:
      return "Windows10";
    case 6:
      switch (osvi.dwMinorVersion) {
        case 3:
          return "Windows8.1";
        case 2:
          return "Windows8";
        case 1:
          return "Windows7";
        case 0:
          return "Windows Vista";
      }
    case 5:
      return "Windows XP";
  }
  return "Unknown";
}
//#endif
