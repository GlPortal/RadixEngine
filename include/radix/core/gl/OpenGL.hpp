#ifndef RADIX_CORE_GL_OPENGL_HPP
#define RADIX_CORE_GL_OPENGL_HPP

#ifdef _WIN32
#define RADIX_GL_USE_GLAD
#endif

#if defined(RADIX_GL_USE_LIBEPOXY)
#include <epoxy/gl.h>
#elif defined(RADIX_GL_USE_GLAD)
#include <glad/glad.h>
#else
#error "No OpenGL loader selected"
#endif

namespace radix {
namespace gl {

class OpenGL {
public:
  static void initialize();
  static int version();
  static bool hasExtension(const char*);
};

} /* namespace gl */
} /* namespace radix */

#endif /* RADIX_CORE_GL_DEBUG_OUTPUT_HPP */
