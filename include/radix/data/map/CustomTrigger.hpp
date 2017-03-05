#ifndef RADIX_CUSTOM_TRIGGER_HPP
#define RADIX_CUSTOM_TRIGGER_HPP

#include <functional>
#include <string>

#include <tinyxml2.h>

namespace radix {

struct CustomTrigger {
  std::string TYPE;
  std::function<void(Entity &trigger, tinyxml2::XMLElement *xmlElement)> loadFunction;
};

} /* namespace radix */

#endif /* RADIX_CUSTOM_TRIGGER_HPP */
