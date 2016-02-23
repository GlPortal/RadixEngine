#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>
#include <string>

namespace radix {

class Input {
public:
  static void keyPressed(int key, int mod);
  static void keyReleased(int key, int mod);
  static bool isKeyDown(int key);
  static std::string getCharBuffer();
  static void addToBuffer(const std::string &character);
  static void clearBuffer();
  static void truncateCharBuffer();
  static void clear();
private:
  static std::vector<bool> keystates;
  static std::string charbuffer;
};

} /* namespace radix */

#endif /* INPUT_HPP */
