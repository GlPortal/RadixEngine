#ifndef RADIX_XMLSCREENLOADER_HPP
#define RADIX_XMLSCREENLOADER_HPP

#include <memory.h>
#include <tinyxml2.h>
#include "Screen.hpp"

namespace radix {

class XMLScreenLoader {
public:
  static std::shared_ptr<Screen> loadScreen(const std::string &path);
private:
  static std::vector<Text> loadText(tinyxml2::XMLHandle &rootHandle);
};
} /* namespace radix */

#endif //RADIX_XMLSCREENLOADER_HPP
