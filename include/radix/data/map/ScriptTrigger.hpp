#ifndef RADIX_SCRIPT_TRIGGER_HPP
#define RADIX_SCRIPT_TRIGGER_HPP

#include <string>

#include <radix/data/map/BaseTrigger.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

  class ScriptTrigger: public BaseTrigger {
  private:
    std::string code;
  public:
    static const std::string TYPE;
    ScriptTrigger(const std::string &code);
    void addAction(Entity& trigger);
  };

} /* namespace radix */

#endif /* RADIX_SCRIPT_TRIGGER_HPP */
