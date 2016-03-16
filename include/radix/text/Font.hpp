#ifndef RADIX_FONT_HPP
#define RADIX_FONT_HPP

#include <map>
#include <string>

#include <radix/text/Glyph.hpp>

namespace radix {

class Font {
public:
  int num_chars;
  float size;

  const Glyph& getGlyph(int index) const {
    return letters.at(index);
  }

  const int getStringLength(std::string string) {
    int length = 0;
    const char *array = string.c_str();
    for (unsigned int i = 0; i < string.length(); i++) {
      char c = array[i];

      const Glyph &letter = getGlyph(c);

      length = length + (letter.width * this->size);
    }
    return length;
  }

  std::map<int, Glyph> letters;
};

} /* namespace radix */

#endif /* RADIX_FONT_HPP */
