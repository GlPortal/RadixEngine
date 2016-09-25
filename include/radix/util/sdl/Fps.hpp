//
// Created by geert on 9/25/16.
//

#ifndef GLPORTAL_FPS_HPP
#define GLPORTAL_FPS_HPP

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

#endif //GLPORTAL_FPS_HPP
