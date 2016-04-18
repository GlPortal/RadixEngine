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
  class CallbackHolder final : public CallbackPointer {
  private:
    EventDispatcher *dispatcher;

  public:
    CallbackHolder() :
      dispatcher(nullptr) {
    }
    CallbackHolder(EventDispatcher *dispatcher, EventType &et, CallbackList::iterator &it) :
      CallbackPointer(et, it),
      dispatcher(dispatcher) {
    }

    // Handy call operator to directly call the callback
    void operator()(const Event &e) {
      (*second)(e);
    }

    // No copy
    CallbackHolder(CallbackHolder&) = delete;
    CallbackHolder& operator=(CallbackHolder&) = delete;

    // Allow movement
    CallbackHolder(CallbackHolder &&o) :
      CallbackPointer(o),
      dispatcher(o.dispatcher) {
      o.dispatcher = nullptr;
    }
    CallbackHolder& operator=(CallbackHolder &&o) {
      CallbackPointer::operator=(o);
      dispatcher = o.dispatcher;
      o.dispatcher = nullptr;
      return *this;
    }

    ~CallbackHolder() {
      if (dispatcher) {
        dispatcher->unobserve(*this);
      }
    }
  };

private:
  ObserverMap observerMap;
  CallbackList wildcardObservers;

public:
  EventDispatcher();


  /* Observers */
  CallbackHolder  observe(EventType type, const Callback &method);
  CallbackPointer observeRaw(EventType type, const Callback &method);

  void unobserve(const CallbackPointer &ptr);
  template<class... CPTypes>
  void unobserve(CallbackPointer cb0, const CPTypes & ...cbN) {
    unobserve(cb0);
    unobserve(cbN...);
  }


  /* Wildcard observers */
  CallbackHolder  observeWildcard(const Callback &method);
  CallbackPointer observeWildcardRaw(const Callback &method);

  void unobserveWildcard(const CallbackPointer &ptr);
  // Templated unobserveWildcard intentionally unimplemented: wildcard is a debug feature.


  /* Event dispatching */
  void dispatch(const Event &event);
};

} /* namespace radix */
#endif /* DISPATCHER_HPP */
