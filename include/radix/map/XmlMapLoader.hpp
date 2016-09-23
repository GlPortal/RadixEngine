#ifndef RADIX_XMLMAPLOADER_HPP
#define RADIX_XMLMAPLOADER_HPP

#include <string>

#include <tinyxml2.h>

#include <radix/map/MapLoader.hpp>

namespace radix {

/** \class XmlMapLoader
 *  Load a map in GlPortal XML format.
 *  @ref map-format-spec
 */
class XmlMapLoader : public MapLoader {
private:
  tinyxml2::XMLHandle rootHandle;

public:
  XmlMapLoader(World&);
  /**
   * Load data from XML into World.
   */
  void load(const std::string &path);

private:
  void extractMaterials();
  void extractSpawn();
  void extractLights();
  void extractDoor();
  void extractWalls();
  void extractAcids();
  void extractTriggers();
  void extractModels();
};

} /* namespace radix */

#endif /* RADIX_XMLMAPLOADER_HPP */
