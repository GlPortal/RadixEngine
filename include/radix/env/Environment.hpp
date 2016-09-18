#ifndef RADIX_ENVIRONMENT_HPP
#define RADIX_ENVIRONMENT_HPP

#include <string>

#include <radix/env/Config.hpp>

namespace radix {

/** @brief Manager for config
 *
 *  Manages the configuration of the the game
 */

class Environment {
private:
  static Config config;
  static std::string dataDir;

public:
  static void init();
  static Config& getConfig();
  static void initializeConfig();
  static std::string getDataDir();
  static void setDataDir(const std::string &string);
};

} /* namespace radix */

#endif /* RADIX_ENVIRONMENT_HPP */
