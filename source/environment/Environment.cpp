#include <radix/environment/Environment.hpp>
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>

using namespace cppfs;

namespace radix {
  Environment::Environment(std::string dataPath) :
    dataPath(dataPath) {
    FileHandle fileHandle = fs::open(dataPath);
    if (!fileHandle.isDirectory()) {
      throw std::runtime_error(std::string("The supplied environment path is not a directory."));
    }
    if (!fileHandle.exists()) {
      throw std::runtime_error(std::string("The supplied environment path does not exist."));
    }
  }

  std::string Environment::getDataPath() {
    return dataPath;
  }
} /* namespace radix */
