#include <radix/core/event/observer/FlagObserver.hpp>

#include <iostream>

namespace radix {

FlagObserver::FlagObserver(bool &flag, bool resultingStatus) {
  this->flag = &flag;
  this->resultingStatus = resultingStatus;
}

void FlagObserver::execute() {
  *flag = resultingStatus;
}

} /* namespace radix */
