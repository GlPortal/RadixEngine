#ifndef VBO_HPP
#define VBO_HPP

#include <vector>

#include <epoxy/gl.h>

namespace glPortal {

class VBO {
protected:
  std::size_t size;
  GLuint id;

public:
  // Ctor / dtor
  VBO();
  VBO(std::size_t size, GLenum usage = GL_STATIC_DRAW);
  ~VBO();
  // No copy
  VBO(const VBO&) = delete;
  VBO& operator=(const VBO&) = delete;
  // Move
  VBO(VBO&&);
  VBO& operator=(VBO&&);
  
  operator GLuint() const { return id; }
  void setSize(std::size_t size, GLenum usage = GL_STATIC_DRAW);
  std::size_t getSize() const {
    return size;
  }

  template <typename T>
  void setData(const std::vector<T> &data, GLenum usage = GL_STATIC_DRAW) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    size = data.size()*sizeof(T);
    glBufferData(GL_ARRAY_BUFFER, size, data.data(), usage);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  template <typename T>
  void setData(const T *data, std::size_t count, GLenum usage = GL_STATIC_DRAW) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    size = count*sizeof(T);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  template <typename T>
  void update(const std::vector<T> &data, std::size_t count = 0, std::size_t offset = 0) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    if (count == 0) {
      count = data.size();
    }
    glBufferSubData(GL_ARRAY_BUFFER, offset, count*sizeof(T), data.data());
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  template <typename T>
  void update(const T *data, std::size_t count, std::size_t offset = 0) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, count*sizeof(T), data);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  void bind() const;
};

} /* namespace glPortal */

#endif /* VBO_HPP */