#include <radix/environment/Environment.hpp>

namespace radix {
  Environment::Environment(std::string dataPath) :
    dataPath(dataPath) {
  }

  std::string Environment::getDataPath() {
    return dataPath;
  }
} /* namespace radix */
