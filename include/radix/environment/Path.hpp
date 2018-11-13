#ifndef RADIX_PATH_HPP
#define RADIX_PATH_HPP

#include <string>

namespace radix {
  class Path {
  private:
    const std::string path;
  public:
    Path(std::string path);
    std::string toString();
  };

} /* namespace radix */

#endif /* RADIX_PATH_HPP */
