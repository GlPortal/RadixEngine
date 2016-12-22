#ifndef RADIX_MESHLOADER_HPP
#define RADIX_MESHLOADER_HPP

#include <string>
#include <map>

#include <radix/map/model/Mesh.hpp>

struct aiMesh;

namespace radix {
class Entity;

class MeshLoader {
public:
  static Mesh& getMesh(const std::string &path);
  static Mesh getPortalBox(const Entity &wall);
  static Mesh getSubPlane(int x, int y, int width, int height, int w, int h);
private:
  static Mesh uploadMesh(const aiMesh *mesh);
  static std::map<std::string, Mesh> meshCache;
};

} /* namespace radix */

#endif /* RADIX_MESHLOADER_HPP */
