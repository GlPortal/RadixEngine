#ifndef RADIX_XML_TRIGGER_HELPER_HPP
#define RADIX_XML_TRIGGER_HELPER_HPP

#include <string>

#include <tinyxml2.h>

#include <radix/core/math/Vector3f.hpp>
#include <radix/Entity.hpp>
#include <radix/data/map/XmlHelper.hpp>

namespace radix {

  class XmlTriggerHelper {
  public:
    static void extractTriggerActions(Entity& trigger, tinyxml2::XMLElement* xmlElement);
    static void addDeathAction(Entity& trigger);
    static void addWinAction(Entity& trigger);
    static void addRadiationAction(Entity& trigger);
    static void addMapAction(std::string filename, Entity& trigger);
  };

} /* namespace radix */

#endif /* RADIX_XML_TRIGGER_HELPER_HPP */
