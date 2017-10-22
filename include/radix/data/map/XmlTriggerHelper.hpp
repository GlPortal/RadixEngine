#ifndef RADIX_XML_TRIGGER_HELPER_HPP
#define RADIX_XML_TRIGGER_HELPER_HPP

#include <string>
#include <list>

#include <tinyxml2.h>

#include <radix/core/math/Vector3f.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/data/map/XmlHelper.hpp>
#include <radix/data/map/CustomTrigger.hpp>

namespace radix {

  class XmlTriggerHelper {
  public:
    static void extractTriggerActions(entities::Trigger &trigger, tinyxml2::XMLElement* xmlElement, const std::list<CustomTrigger>& customTriggers);
    static void extractMapTriggerActions(entities::Trigger &trigger, tinyxml2::XMLElement *xmlElement);
    static void extractAudioTriggerActions(entities::Trigger &trigger, tinyxml2::XMLElement *xmlElement);
    static void extractDestinationTriggerActions(entities::Trigger &trigger,
                                                 tinyxml2::XMLElement *xmlElement);
    static void extractScreenTriggerActions(entities::Trigger &trigger, tinyxml2::XMLElement *xmlElement);
  };

} /* namespace radix */

#endif /* RADIX_XML_TRIGGER_HELPER_HPP */
