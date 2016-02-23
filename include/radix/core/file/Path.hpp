#ifndef PATH_HPP
#define PATH_HPP

#include <string>

namespace radix {

class Path {
public:
  static std::string GetDirectorySeparator();

  // convert path from unix paths used throughout the code
  static std::string FromUnixPath(const std::string & unixPath);
};

} /* namespace radix */

#endif /* PATH_HPP */
