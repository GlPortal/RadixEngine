#ifndef RADIX_MESHLOADER_HPP
#define RADIX_MESHLOADER_HPP

#include <radix/data/model/Mesh.hpp>

#include <map>
#include <string>

struct aiMesh;

namespace radix {
class Entity;

class MeshLoader {
public:
  /**
   * @brief The VertexAttribInfo struct
   * Holder for VertexAttrib data
   */
  struct VertexAttribInfo {
    unsigned int index; /** < attrib index */
    int channels;       /** < number of channels of vertex */
    unsigned int type;  /** < type vertex */
    void *offset;       /** < offset pointer */
  };

  /**
   * @brief getMesh read mesh file and upload it to GPU
   * @param path    mesh file name
   * @return        get handler to GPU buffer
   */
  static Mesh &getMesh(const std::string &path);

  /**
   * @brief getPortalBox Create and upload Portal Box
   * @param wall         Wall specfic Info
   * @return             get created Portal Box
   */
  static Mesh getPortalBox(const Entity &wall);

  /**
   * @brief getSubPlane get sub plane
   * @param x           starting letter x
   * @param y           starting letter x
   * @param width       width of letter
   * @param height      height of letter
   * @param w           texture width
   * @param h           texture height
   * @return            return sub plane mesh
   */
  static Mesh getSubPlane(int x, int y, int width, int height, int w, int h);

private:
  /**
   * @brief uploadMesh upload data to GPU memory
   * @param mesh       pointer to aiMesh
   * @return           Mesh object of uploaded data
   */
  static Mesh uploadMesh(const aiMesh *mesh);

  using specVector = std::vector<VertexAttribInfo>;

  /**
   * @brief createVBO create Vertex Buffer Object and upload data
   * @param type      OpenGL buffer type
   * @param meshSize  Buffer size
   * @param meshPtr   Pointer to Mesh data
   * @param vtxSize   [optional] for spec vertex size
   * @param map       [optional] for spec vector of vertex attrib
   * @return          OpenGL Handler to created buffer
   * @link
   * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
   * @link
   * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
   * @link
   * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
   */
  static unsigned int createVBO(const unsigned int type,
                                const unsigned int meshSize,
                                const void *meshPtr, const int vtxSize = 0,
                                const specVector &map = {});

  static std::map<std::string, Mesh> meshCache; /** < Mesh chached file */
};

} /* namespace radix */

#endif /* RADIX_MESHLOADER_HPP */
