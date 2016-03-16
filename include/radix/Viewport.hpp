#ifndef RADIX_VIEWPORT_HPP
#define RADIX_VIEWPORT_HPP

namespace radix {

struct Viewport {
  virtual void getSize(int *w, int *h) const = 0;
};

} /* namespace radix */

#endif /* RADIX_VIEWPORT_HPP */
