#ifndef RADIX_MAP_TRIGGER_HPP
#define RADIX_MAP_TRIGGER_HPP

#include <string>

#include <radix/data/map/BaseTrigger.hpp>
#include <radix/core/math/Vector3f.hpp>

namespace radix {

  class MapTrigger: public BaseTrigger {
  private:
    std::string filePath;
    bool loop;
  public:
    static const std::string TYPE;
    MapTrigger(std::string filePath);
    void addAction(Entity& trigger);
  };

} /* namespace radix */

#endif /* RADIX_MAP_TRIGGER_HPP */
