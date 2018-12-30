#include <radix/core/event/EventDispatcher.hpp>
#include <radix/env/Util.hpp>
#include <ciso646>

static const char *Tag = "EventDispatcher";

namespace radix {

EventDispatcher::EventDispatcher() :
  debugLogLevel(DebugLogLevel::Silent) {
}

void EventDispatcher::dispatch(const Event &event) {
  ObserverMap::iterator it;
  it = observerMap.find(event.getType());
  if (debugLogLevel == DebugLogLevel::DispatchedEvents or
      debugLogLevel == DebugLogLevel::DispatchedEventsRepr) {
    unsigned int obsCount = 0;
    if (it != observerMap.end()) {
      obsCount = it->second.size();
    }
    Util::Log(Verbose, Tag) << "Dispatch " << event.getTypeName() << " to " << obsCount <<
                               '+' << wildcardObservers.size() << " observers";
    if (debugLogLevel == DebugLogLevel::DispatchedEventsRepr) {
      Util::Log(Verbose, Tag) << event.debugStringRepr();
    }
  }
  if (it != observerMap.end()) {
    CallbackList &observers = it->second;
    for (Callback &f : observers) {
      f(event);
    }
  }
  for (Callback &f : wildcardObservers) {
    f(event);
  }
}

EventDispatcher::CallbackHolder EventDispatcher::addObserver(
        EventType type, const Callback &method) {
  ObserverMap::iterator it;
  it = observerMap.find(type);
  if (it != observerMap.end()) {
    it->second.push_back(method);
    return CallbackHolder(this, type, --it->second.end());
  } else {
    auto newIt = observerMap.emplace(std::piecewise_construct,
                                     std::forward_as_tuple(type),
                                     std::forward_as_tuple()).first;
    CallbackList &observers = newIt->second;
    observers.push_back(method);
    return CallbackHolder(this, type, --observers.end());
  }
}

EventDispatcher::CallbackPointer EventDispatcher::addObserverRaw(
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

void EventDispatcher::removeObserver(const CallbackPointer &ptr) {
  ObserverMap::iterator it;
  it = observerMap.find(ptr.first);
  if (it != observerMap.end()) {
    CallbackList &observers = it->second;
    observers.erase(ptr.second);
  }
}

EventDispatcher::CallbackHolder EventDispatcher::observeWildcard(const Callback &method) {
  wildcardObservers.push_back(method);
  EventType et = 0;
  return CallbackHolder(this, et, --wildcardObservers.end());
}

EventDispatcher::CallbackPointer EventDispatcher::observeWildcardRaw(const Callback &method) {
  wildcardObservers.push_back(method);
  return std::make_pair(0, --wildcardObservers.end());
}

void EventDispatcher::unobserveWildcard(const CallbackPointer &ptr) {
  wildcardObservers.erase(ptr.second);
}

} /* namespace radix */
