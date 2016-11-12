#include <radix/PlayerTask.hpp>
#include <radix/env/Util.hpp>

namespace radix {

PlayerTestTask::PlayerTestTask() {
  task = [] () {
    Util::Log() << "Test";
  };
}

std::string PlayerTestTask::getName() {
  return "PlayerTestTask";
}

} /* namespace radix */