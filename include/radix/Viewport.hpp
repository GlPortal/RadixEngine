#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

namespace radix {

struct Viewport {
  virtual void getSize(int *w, int *h) const = 0;
};

} /* namespace radix */

#endif /* VIEWPORT_HPP */
