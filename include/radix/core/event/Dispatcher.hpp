#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <functional>
#include <list>
#include <map>
#include <utility>

#include <radix/core/event/Event.hpp>

namespace radix {

class Observer;

class Dispatcher {
public:
  typedef std::list<std::function<void()>> CallbackList;
  typedef std::map<Event, CallbackList> ObserverMap;
  typedef std::pair<Event, CallbackList::iterator> CallbackPointer;

private:
  ObserverMap eventObserverMap;
  
  friend Observer;
  CallbackPointer addObserver(Event event, const std::function<void()> &method);
  void removeObserver(CallbackPointer &ptr);

public:
  Dispatcher();
  void dispatch(Event event);
};

} /* namespace radix */
#endif /* DISPATCHER_HPP */
