#include <radix/environment/Path.hpp>

namespace radix {
  Path::Path(std::string path) :
    path(path) {
  }

  std::string Path::toString() {
    return path;
  }
} /* namespace radix */
