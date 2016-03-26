#ifndef RADIX_NULL_INPUT_SOURCE_HPP
#define RADIX_NULL_INPUT_SOURCE_HPP

#include <radix/input/InputSource.hpp>

namespace radix {

class NullInputSource : public InputSource {
public:
  void processEvents() {}

  void keyPressed(int, int) {}
  void keyReleased(int, int) {}
  bool isKeyDown(int) { return false; }
  std::string getCharBuffer() { return ""; }
  void addToBuffer(const std::string&) {}
  void clearBuffer() {}
  void truncateCharBuffer() {}
  void clear() {}
};

} /* namespace radix */

#endif /* RADIX_NULL_INPUT_SOURCE_HPP */
