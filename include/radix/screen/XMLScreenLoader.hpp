#ifndef RADIX_XMLSCREENLOADER_HPP
#define RADIX_XMLSCREENLOADER_HPP

#include <bits/unique_ptr.h>
#include <tinyxml2.h>
#include "Screen.hpp"

namespace radix {

class XMLScreenLoader {
public:
  static std::unique_ptr<Screen> loadScreen(const std::string &path);
private:
  static std::vector<Text> loadText(tinyxml2::XMLHandle &rootHandle);
};
} /* namespace radix */

#endif //RADIX_XMLSCREENLOADER_HPP
