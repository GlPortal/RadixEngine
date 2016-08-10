#include <radix/core/file/Path.hpp>

#include <string>

#ifndef _WIN32
#include <sys/stat.h>
#endif

using namespace std;

namespace radix {

string Path::GetDirectorySeparator() {
#ifndef _WIN32
  return "/";
#else
  return "\\";
#endif
}

bool Path::DirectoryExist(std::string &directory) {
#ifndef _WIN32
  struct stat sb;

  if (stat(directory.c_str(), &sb) == 0 and S_ISDIR(sb.st_mode)) {
    return true;
  }
  return false;
#else
  return true;
#endif
}

string Path::FromUnixPath(const string &unixPath) {
  string path(unixPath);
#ifdef _WIN32
  for (unsigned int i = 0; i < path.size(); ++i) {
    if (path.at(i) == '/') {
      path.at(i) = '\\';
    }
  }
#endif
  return path;
}

} /* namespace radix */
