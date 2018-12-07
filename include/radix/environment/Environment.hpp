#ifndef RADIX_ENVIRONMENT_HPP
#define RADIX_ENVIRONMENT_HPP

#include <string>
#include <memory>
#include <radix/env/Config.hpp>

namespace radix {
  class Environment {
  private:
    const std::shared_ptr<Config> config;
    const std::string dataPath;
  public:
    Environment(std::string dataPath);
    std::shared_ptr<Config> getConfig();
    std::string getDataPath();
  };

} /* namespace radix */

#endif /* RADIX_ENVIRONMENT_HPP */
