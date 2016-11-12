#include <radix/PlayerTask.hpp>

namespace radix {

PlayerTestTask::PlayerTestTask() {
  task = [] (World &world) {
    Util::Log() << "Test";
  };
}

std::string PlayerTestTask::getName() {
  return "PlayerTestTask";
}

} /* namespace radix */