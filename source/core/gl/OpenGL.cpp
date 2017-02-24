#include <radix/core/gl/OpenGL.hpp>

#if defined(RADIX_GL_USE_GLAD)
#include <cstring>
#include <stdexcept>
#include <set>
#endif

#include <ciso646>
#include <stdexcept>

namespace radix {
namespace gl {

#if defined(RADIX_GL_USE_GLAD)
static std::set<const char *> extensions;
#endif

void OpenGL::initialize() {
#if defined(RADIX_GL_USE_LIBEPOXY)
  // libepoxy initializes lazily
#elif defined(RADIX_GL_USE_GLAD)
  if (!gladLoadGL()) {
    throw std::runtime_error("GLAD initialization failed");
  }
  GLint n, i;
  glGetIntegerv(GL_NUM_EXTENSIONS, &n);
  for (i = 0; i < n; i++) {
    extensions.insert(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
  }
#endif
}

int OpenGL::version() {
#if defined(RADIX_GL_USE_LIBEPOXY)
  return epoxy_gl_version();
#elif defined(RADIX_GL_USE_GLAD)
  return GLVersion.major * 10 + GLVersion.minor;
#endif
}

bool OpenGL::hasExtension(const char *ext) {
#if defined(RADIX_GL_USE_LIBEPOXY)
  return epoxy_has_gl_extension(ext);
#elif defined(RADIX_GL_USE_GLAD)
  return extensions.find(ext) != extensions.end();
#endif
}

} /* namespace gl */
} /* namespace radix */
