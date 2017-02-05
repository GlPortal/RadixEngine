#include <radix/core/gl/VBO.hpp>

#include <glad/glad.h>

namespace radix {

constexpr static GLenum getGlUsage(const VBO::Usage usage) {
  switch (usage & 3) {
  case VBO::Static:
    switch (usage >> 2) {
    case VBO::Draw:
      return GL_STATIC_DRAW;
    case VBO::Read:
      return GL_STATIC_READ;
    case VBO::Copy:
      return GL_STATIC_COPY;
    default:
      ;
    }
    return 0;
  case VBO::Dynamic:
    switch (usage >> 2) {
    case VBO::Draw:
      return GL_DYNAMIC_DRAW;
    case VBO::Read:
      return GL_DYNAMIC_READ;
    case VBO::Copy:
      return GL_DYNAMIC_COPY;
    default:
      ;
    }
    return 0;
  case VBO::Stream:
    switch (usage >> 2) {
    case VBO::Draw:
      return GL_STREAM_DRAW;
    case VBO::Read:
      return GL_STREAM_READ;
    case VBO::Copy:
      return GL_STREAM_COPY;
    default:
      ;
    }
    return 0;
  default:
    ;
  }
  return 0;
}

VBO::VBO() :
  size(0) {
  glGenBuffers(1, &id);
}

VBO::VBO(std::size_t size, Usage usage) :
  size(0) {
  glGenBuffers(1, &id);
  setSize(size, usage);
}

VBO::VBO(VBO &&other) {
  glDeleteBuffers(1, &id);
  id = other.id; size = other.size;
  other.size = other.id = 0;
}
VBO& VBO::operator=(VBO &&other) {
  glDeleteBuffers(1, &id);
  id = other.id; size = other.size;
  other.size = other.id = 0;
  return *this;
}

VBO::~VBO() {
  glDeleteBuffers(1, &id);
}

void VBO::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::setSize(std::size_t size, Usage usage) {
  GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  this->size = size;
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, getGlUsage(usage));
  glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
}

void VBO::setData(const void *data, std::size_t count, Usage usage) {
  GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  this->size = count;
  glBufferData(GL_ARRAY_BUFFER, count, data, getGlUsage(usage));
  glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
}

void VBO::update(const void *data, std::size_t count, std::size_t offset) {
  GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferSubData(GL_ARRAY_BUFFER, offset, count, data);
  glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
}

} /* namespace radix */
