#ifndef RADIX_MAPLOADER_HPP
#define RADIX_MAPLOADER_HPP

#include <string>

namespace radix {

class Entity;
class Mesh;
class World;

class MapLoader {
protected:
  World &world;
  MapLoader(World&);

public:
  virtual void load(const std::string &path) = 0;
};

} /* namespace radix */

#endif /* RADIX_MAPLOADER_HPP */
