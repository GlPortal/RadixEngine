#include <radix/data/model/MeshLoader.hpp>
#include <radix/Entity.hpp>
#include <radix/env/Environment.hpp>
#include <radix/component/Transform.hpp>
#include <radix/core/gl/OpenGL.hpp>
#include <radix/core/gl/TightDataPacker.hpp>

#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace radix {

std::map<std::string, Mesh> MeshLoader::meshCache = {};

Mesh &MeshLoader::getMesh(const std::string &path) {
  // check if mesh is cahced
  auto it = meshCache.find(path);
  if (it != meshCache.end()) {
    return it->second;
  }

  const std::string filePath = Environment::getDataDir() + "/meshes/" + path;
  Assimp::Importer importer;
  // flags to read mesh
  // - aiProcess_Triangulate      : Triangulates all faces of all meshes.
  // - aiProcess_GenNormals       : Generates normals for all faces of all meshes.
  // - aiProcess_CalcTangentSpace : Calculates the tangents and bitangents for
  //   the imported meshes.
  // http://assimp.sourceforge.net/lib_html/postprocess_8h.html
  unsigned int flags =
      aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
  const auto container = importer.ReadFile(filePath, flags);
  const aiMesh *mesh = container->mMeshes[0];
  // Upload mesh to GPU
  Mesh meshObject = uploadMesh(mesh);

  auto inserted = meshCache.insert(std::make_pair(path, meshObject));
  return inserted.first->second;
}

using specVector = std::vector<MeshLoader::VertexAttribInfo>;

GLuint MeshLoader::createVBO(const GLenum type,
                             const GLuint meshSize,
                             const GLvoid* meshPtr,
                             const int vtxSize,
                             const specVector& map) {
  GLuint VBO = 0;
  // Create buffer OpenGL handler
  glGenBuffers(1, &VBO);
  // assign buffer to OpenGL type
  glBindBuffer(type, VBO);
  // upload buffer to GPU
  glBufferData(type, meshSize, meshPtr, GL_STATIC_DRAW);
  for(auto item : map) {
      glVertexAttribPointer(item.index, item.channels, item.type, GL_FALSE, vtxSize, item.offset);
      glEnableVertexAttribArray(item.index);
  }
  // assign buffer to default
  glBindBuffer(type, 0);
  // return OpenGL handler
  return VBO;
}

Mesh MeshLoader::uploadMesh(const aiMesh *mesh) {
  Mesh model;

  // Generate vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create vertex indices VBO
  {
    // Store face indices in an array
    std::unique_ptr<unsigned int[]> faceArray(new unsigned int[mesh->mNumFaces * 3]);
    for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
      const aiFace &face = mesh->mFaces[f];
      std::memcpy(&faceArray[f * 3], face.mIndices, 3 * sizeof(face.mIndices[0]));
    }
    // Prepare vertex indices VBO
    createVBO(GL_ELEMENT_ARRAY_BUFFER,
              sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray.get());
  }

  // Cache vertex attributes existence to avoid dereferencing and calling every
  // time
  bool hasPositions = mesh->HasPositions(),
       hasTexCoords = mesh->HasTextureCoords(0),
       hasNormals = mesh->HasNormals(),
       hasTangents = mesh->HasTangentsAndBitangents();

  // Use them to determine the vertex definition size
  unsigned int vtxSize = +(hasPositions ? sizeof(float) * 3 : 0) +
                          (hasTexCoords ? sizeof(float) * 2 : 0) +
                          (hasNormals ? sizeof(float) * 3 : 0) +
                          (hasTangents ? sizeof(float) * 3 : 0);

  // Create a buffer to store vertex data, with determined size since we now
  // know it
  TightDataPacker data(mesh->mNumVertices * vtxSize);

  if (hasPositions) {
    // Since we keep the vertices in the model, better determine the storage
    // size once and for all
    model.vertices.resize(mesh->mNumVertices);
  }

  // Process each vertex and store its data
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    if (hasPositions) {
      const aiVector3D &v = mesh->mVertices[i];
      model.vertices[i] = Vector3f(v.x, v.y, v.z);
      data << v.x << v.y << v.z;
    }

    if (hasTexCoords) {
      const aiVector3D &t = mesh->mTextureCoords[0][i];
      data << t.x
           << 1.f - t.y; // Y must be flipped due to OpenGL's coordinate system
    }

    if (hasNormals) {
      const aiVector3D &n = mesh->mNormals[i];
      data << n.x << n.y << n.z;
    }

    if (hasTangents) {
      const aiVector3D &t = mesh->mTangents[i];
      data << t.x << t.y << t.z;
    }
  }

  // Describe the vertex format we have
  {
    intptr_t offset = 0;
    specVector vertexAttrib;
    if (hasPositions) {
          vertexAttrib.push_back({0, 3, GL_FLOAT, reinterpret_cast<GLvoid*>(offset)});
          offset += sizeof(float) * 3;
    }
    if (hasTexCoords) {
          vertexAttrib.push_back({1, 2, GL_FLOAT, reinterpret_cast<GLvoid*>(offset)});
          offset += sizeof(float) * 2;
    }
    if (hasNormals) {
        vertexAttrib.push_back({2, 3, GL_FLOAT, reinterpret_cast<GLvoid*>(offset)});
        offset += sizeof(float) * 3;
    }
    if (hasTangents) {
        vertexAttrib.push_back({3, 3, GL_FLOAT, reinterpret_cast<GLvoid*>(offset)});
    }
    createVBO(GL_ARRAY_BUFFER, data.getSize(), data.getDataPtr(),
                         static_cast<int>(vtxSize), vertexAttrib);
  }
  glBindVertexArray(0);

  // Store relevant data in the new mesh
  model.handle = static_cast<int>(vao);
  model.numFaces = static_cast<int>(mesh->mNumFaces);

  return model;
}

Mesh MeshLoader::getPortalBox(const Entity &wall) {
  // Generate vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  /* == Static part: vertices, normals and tangents == */
  constexpr unsigned int coordsSize = sizeof(float) * 3,
                         texcSize = sizeof(float) * 2,
                         normalsSize = sizeof(int8_t) * 3,
                         tangentsSize = sizeof(int8_t) * 3,
                         vtxSize = coordsSize + texcSize + normalsSize + tangentsSize,
                         vboSize = vtxSize * 3 /*verts*/ * 2 /*tris*/ * 6; /*faces*/
  TightDataPacker data(vboSize);

  static constexpr float vertices[8][3] = {
      {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f},
      {-0.5f, 0.5f, 0.5f},   {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f},
      {0.5f, 0.5f, -0.5f},   {0.5f, 0.5f, 0.5f}};

  static constexpr uint8_t vi[36] = {
      3, 1, 5, 3, 5, 7, // Front
      7, 5, 4, 7, 4, 6, // Left
      6, 4, 0, 6, 0, 2, // Back
      2, 0, 1, 2, 1, 3, // Right
      2, 3, 7, 2, 7, 6, // Top
      1, 0, 4, 1, 4, 5  // Bottom
  };

  const Transform &t = wall.getComponent<Transform>();
  const Vector3f &scale = t.getScale();

  const float texCoords[8][2] = {
      {0, 0},       {scale.x, 0},       {scale.z, 0},       {0, scale.y},
      {0, scale.z}, {scale.x, scale.y}, {scale.x, scale.z}, {scale.z, scale.y}};

  static constexpr uint8_t ti[36] = {0, 3, 5, 0, 5, 1, 0, 3, 7, 0, 7, 2,
                                     0, 3, 5, 0, 5, 1, 0, 3, 7, 0, 7, 2,
                                     0, 4, 6, 0, 6, 1, 0, 4, 6, 0, 6, 1};

  static constexpr int8_t normals[6][3] = {{0, 0, 1},  {1, 0, 0}, {0, 0, -1},
                                           {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}};

  static constexpr uint8_t ni[36] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
                                     2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
                                     4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5};

  static const int8_t tangents[6][3] = {{0, 1, 0},  {0, 0, 1}, {0, -1, 0},
                                        {0, 0, -1}, {1, 0, 0}, {-1, 0, 0}};

  static constexpr uint8_t tai[36] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
                                      2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
                                      4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5};
  constexpr unsigned int texCoordsOffset = coordsSize,
                         normalsOffset = texCoordsOffset + texcSize,
                         tangentsOffset = normalsOffset + normalsSize;

  for (int i = 0; i < 36; ++i) {
    const float *v = vertices[vi[i]];
    data << v[0] << v[1] << v[2];

    const float *t = texCoords[ti[i]];
    data << t[0] << t[1];

    const int8_t *n = normals[ni[i]];
    data << n[0] << n[1] << n[2];

    const int8_t *ta = tangents[tai[i]];
    data << ta[0] << ta[1] << ta[2];
  }

  Mesh mesh;
  mesh.vertices.resize(8);

  for (unsigned int i = 0; i < 8; ++i) {
    const float *v = vertices[i];
    mesh.vertices[i] = Vector3f(v[0], v[1], v[2]);
  }

  assert(data.getSize() == vboSize);

  createVBO(GL_ARRAY_BUFFER, vboSize, data.getDataPtr(), vtxSize,
   {{0, 3, GL_FLOAT, nullptr},                                      // Vertices
    {1, 2, GL_FLOAT, reinterpret_cast<GLvoid *>(texCoordsOffset)},  // Tex coords
    {2, 3, GL_BYTE, reinterpret_cast<GLvoid *>(normalsOffset)},     // Normals
    {3, 3, GL_BYTE, reinterpret_cast<GLvoid *>(tangentsOffset)}});  // Tangents
  // unbind Vertex Array Object (VAO)
  glBindVertexArray(0);

  // Store relevant data in the new mesh
  mesh.handle = static_cast<int>(vao);
  mesh.numFaces = 12;

  return mesh;
}

Mesh MeshLoader::getSubPlane(int x, int y, int width, int height, int w,
                             int h) {
  // Generate vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  static constexpr int8_t vertices[4][3] = {
      {0, -1, 0}, {1, -1, 0}, {1, 0, 0}, {0, 0, 0}
  };

  const float texCoords[4][2] = {
      {static_cast<float>(x) / w,         static_cast<float>(y) / h},
      {static_cast<float>(x + width) / w, static_cast<float>(y) / h},
      {static_cast<float>(x + width) / w, static_cast<float>(y + height) / h},
      {static_cast<float>(x) / w,         static_cast<float>(y + height) / h}
  };

  static constexpr uint8_t vi[6] = {0, 1, 3, 2, 3, 1}, ti[6] = {3, 2, 0, 1, 0, 2};

  constexpr unsigned int vtxSize = 3 * sizeof(int8_t) + 2 * sizeof(float);
  TightDataPacker data(6 * vtxSize);
  for (int i = 0; i < 6; ++i) {
    const int8_t *v = vertices[vi[i]];
    data << v[0] << v[1] << v[2];
    const float *t = texCoords[ti[i]];
    data << t[0] << t[1];
  }

  assert(data.getSize() == 6 * vtxSize);

  // Put the vertex buffer into the VAO
  createVBO(GL_ARRAY_BUFFER, data.getSize(), data.getDataPtr(), vtxSize,
            { {0, 3, GL_BYTE, nullptr},
              {1, 2, GL_FLOAT, reinterpret_cast<GLvoid *>(3 * sizeof(int8_t))} });
  // Unbind VAO
  glBindVertexArray(0);

  // Store relevant data in the new mesh
  return Mesh{static_cast<int>(vao), 2, {}};
}

} /* namespace radix */
