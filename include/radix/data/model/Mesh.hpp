#ifndef RADIX_MESH_HPP
#define RADIX_MESH_HPP

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

#endif /* RADIX_MESH_HPP */
