#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include <radix/core/math/Vector3f.hpp>

namespace radix {

class Mesh {
public:
  int handle;
  int numFaces;
  std::vector<Vector3f> vertices;
};

} /* namespace radix */

#endif /* MESH_HPP */
