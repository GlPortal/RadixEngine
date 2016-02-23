#include <radix/core/gl/VBO.hpp>

namespace radix {

VBO::VBO() :
  size(0) {
  glGenBuffers(1, &id);
}

VBO::VBO(std::size_t size, GLenum usage) :
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

void VBO::setSize(std::size_t size, GLenum usage) {
  GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  this->size = size;
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
  glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
}

} /* namespace radix */
