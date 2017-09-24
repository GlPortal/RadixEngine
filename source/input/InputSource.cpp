#include <radix/input/InputSource.hpp>

#include <algorithm>

#include <radix/core/event/EventDispatcher.hpp>

using namespace std;

namespace radix {

void InputSource::removeDispatcher(EventDispatcher &d) {
  dispatchers.erase(std::remove_if(dispatchers.begin(), dispatchers.end(), [&d](EventDispatcher &e) {
    return std::addressof(e) == std::addressof(d);
  }), dispatchers.end());
}

} /* namespace radix */
