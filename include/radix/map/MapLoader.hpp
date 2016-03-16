#ifndef RADIX_MAPLOADER_HPP
#define RADIX_MAPLOADER_HPP

#include <string>

#include <tinyxml2.h>

namespace radix {

class Entity;
class Mesh;
class World;

class MapLoader {
private:
  World &world;
  tinyxml2::XMLHandle rootHandle;

public:
  MapLoader(World&);
  
  void load(const std::string &path);

private:
  static Mesh getBox(const Entity &wall);
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

#endif /* RADIX_MAPLOADER_HPP */
