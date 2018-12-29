#ifndef RADIX_ARGUMENTS_HPP
#define RADIX_ARGUMENTS_HPP

#include <string>
#include <cxxopts.hpp>

namespace radix {
  /*
   * Extend this class, add options in the constructor
   * and add getters for your own arguments.
   */
  class Arguments {
  private:
    std::string executableName;
    std::string dataPath;
  public:
    Arguments(int argumentCount, char **argumentVector);
    std::string getDataPath();
    std::string getExecutableName();
  };

} /* namespace radix */

#endif /* RADIX_ARGUMENTS_HPP */
