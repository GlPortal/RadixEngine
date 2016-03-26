#ifndef RADIX_INPUT_SOURCE_HPP
#define RADIX_INPUT_SOURCE_HPP

#include <string>

namespace radix {

class InputSource {
public:
  virtual void processEvents() = 0;

  virtual void keyPressed(int key, int mod) = 0;
  virtual void keyReleased(int key, int mod) = 0;
  virtual bool isKeyDown(int key) = 0;
  virtual std::string getCharBuffer() = 0;
  virtual void addToBuffer(const std::string &character) = 0;
  virtual void clearBuffer() = 0;
  virtual void truncateCharBuffer() = 0;
  virtual void clear() = 0;
};

} /* namespace radix */

#endif /* RADIX_INPUT_SOURCE_HPP */
