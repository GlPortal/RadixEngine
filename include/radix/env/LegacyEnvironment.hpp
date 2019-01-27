#ifndef RADIX_LEGACY_ENVIRONMENT_HPP
#define RADIX_LEGACY_ENVIRONMENT_HPP

#include <string>

#include <radix/env/Config.hpp>

namespace radix {

/** @brief Manager for config
 *
 *  Manages the configuration of the the game
 */

class LegacyEnvironment {
private:
  static Config config;
  static std::string dataDir;

public:
  static void Init();
  static Config& getConfig();
  static void InitializeConfig();
  static std::string getDataDir();
  static void setDataDir(const std::string &string);
};

} /* namespace radix */

#endif /* RADIX_LEGACY_ENVIRONMENT_HPP */
