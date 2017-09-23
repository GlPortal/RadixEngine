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

  Mesh() :
    handle(0),
    numFaces(0) {
  }
  Mesh(int handle, int numFaces, const std::vector<Vector3f> &vertices) :
    handle(handle),
    numFaces(numFaces),
    vertices(vertices) {
  }
};

} /* namespace radix */

#endif /* RADIX_MESH_HPP */
