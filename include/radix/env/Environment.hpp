#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <string>

#include <radix/env/Config.hpp>

namespace radix {

class Environment {
private:
  static Config config;
  static std::string datadir;

public:
  static void init();
  static Config& getConfig();
  static void initializeConfig();
  static std::string getDataDir();
  static void setDataDir(const std::string &string);
};

} /* namespace radix */

#endif /* ENVIRONMENT_HPP */
