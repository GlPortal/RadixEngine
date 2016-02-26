#include <radix/core/event/EventDispatcher.hpp>

namespace radix {

EventDispatcher::EventDispatcher() {
}

void EventDispatcher::dispatch(const Event &event) {
  ObserverMap::iterator it;
  it = observerMap.find(event.getType());
  if (it != observerMap.end()) {
    CallbackList &observers = it->second;
    for (Callback &f : observers) {
      f(event);
    }
  }
}

EventDispatcher::CallbackPointer EventDispatcher::observe(
  EventType type, const Callback &method) {
  ObserverMap::iterator it;
  it = observerMap.find(type);
  if (it != observerMap.end()) {
    it->second.push_back(method);
    return std::make_pair(type, --it->second.end());
  } else {
    auto newIt = observerMap.emplace(std::piecewise_construct,
                                     std::forward_as_tuple(type),
                                     std::forward_as_tuple()).first;
    CallbackList &observers = newIt->second;
    observers.push_back(method);
    return std::make_pair(type, --observers.end());
  }
}

void EventDispatcher::unobserve(const CallbackPointer &ptr) {
  ObserverMap::iterator it;
  it = observerMap.find(ptr.first);
  if (it != observerMap.end()) {
    CallbackList &observers = it->second;
    observers.erase(ptr.second);
  }
}

} /* namespace radix */
