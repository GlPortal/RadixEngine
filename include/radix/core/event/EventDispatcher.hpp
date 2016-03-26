#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <functional>
#include <list>
#include <unordered_map>
#include <utility>

#include <radix/core/event/Event.hpp>

namespace radix {

class EventDispatcher {
public:
  using Callback = std::function<void(const Event&)>;
  using CallbackList = std::list<Callback>;
  using ObserverMap = std::unordered_map<EventType, CallbackList>;
  using CallbackPointer = std::pair<EventType, CallbackList::iterator>;

private:
  ObserverMap observerMap;

public:
  EventDispatcher();


  CallbackPointer observe(EventType type, const Callback &method);

  void unobserve(const CallbackPointer &ptr);
  template<class... CPTypes>
  void unobserve(CallbackPointer cb0, const CPTypes & ...cbN) {
    unobserve(cb0);
    unobserve(cbN...);
  }

  void dispatch(const Event &event);
};

} /* namespace radix */
#endif /* DISPATCHER_HPP */
