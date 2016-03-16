#ifndef RADIX_FONTLOADER_HPP
#define RADIX_FONTLOADER_HPP

#include <map>
#include <string>

#include <radix/text/Font.hpp>

namespace radix {

class FontLoader {
public:
  static Font& getFont(const std::string &path);
private:
  static Font loadFont(const std::string &path, const std::string &name);
  static std::map<std::string, Font> fontCache;
};

} /* namespace radix */

#endif /* RADIX_FONTLOADER_HPP */
