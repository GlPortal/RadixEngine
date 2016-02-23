#ifndef FLAG_OBSERVER_HPP
#define FLAG_OBSERVER_HPP

#include <radix/core/event/Observer.hpp>

namespace radix {

class FlagObserver : public Observer{
private:
  bool resultingStatus;
  bool *flag;

public:
  FlagObserver(bool &flag, bool resultingStatus);
  void execute();
};

} /* namespace radix */

#endif /* FLAG_OBSERVER_HPP */
