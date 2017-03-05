#ifndef RADIX_XMLMAPLOADER_HPP
#define RADIX_XMLMAPLOADER_HPP

#include <string>
#include <list>

#include <tinyxml2.h>

#include <radix/data/map/MapLoader.hpp>
#include <radix/data/map/CustomTrigger.hpp>

namespace radix {

/** \class XmlMapLoader
 *  Load a map in GlPortal XML format.
 *  @ref map-format-spec
 */
class XmlMapLoader : public MapLoader {
private:
  tinyxml2::XMLHandle rootHandle;
  std::list<CustomTrigger> customTriggers;
public:
  XmlMapLoader(World&, const std::list<CustomTrigger>&);
  /**
   * Load data from XML into World.
   */
  void load(const std::string &path);

private:
  void extractMaterials();
  /**
   * Extract a spawn element containing its rotation and position elements
   */
  void extractSpawn();
  void extractLights();
  void extractDoor();
  void extractWalls();
  void extractAcids();
  void extractDestinations();
  void extractTriggers();
  void extractModels();
};

} /* namespace radix */

#endif /* RADIX_XMLMAPLOADER_HPP */
