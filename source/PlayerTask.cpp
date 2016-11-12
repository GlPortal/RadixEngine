#include <radix/PlayerTask.hpp>
#include <radix/env/Util.hpp>

namespace radix {

PlayerTestTask::PlayerTestTask() {
  task = [] () {
    Util::Log() << "Test";
  };
}

} /* namespace radix */