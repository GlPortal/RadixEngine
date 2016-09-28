//
// Created by geert on 9/25/16.
//

#ifndef RADIX_FPS_HPP
#define RADIX_FPS_HPP

namespace radix {

class Fps {
public:
  Fps();
  int getFps();
  void countCycle();
private:
  unsigned int lastFpsTime;
  int fps;
  int skipped;
  int rendered;
};
} /* namespace radix */

#endif /* RADIX_FPS_HPP */
