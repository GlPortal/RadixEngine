#include <radix/core/gl/OpenGL.hpp>

#if defined(RADIX_GL_USE_GLAD)
#include <cstring>
#endif

namespace radix {
namespace gl {

#if defined(RADIX_GL_USE_GLAD)
static const char *extensions;
#endif

void OpenGL::initialize() {
#if defined(RADIX_GL_USE_LIBEPOXY)
  // libepoxy initializes lazily
#elif defined(RADIX_GL_USE_GLAD)
  if (!gladLoadGL()) {
    throw std::runtime_exception("GLAD initialization failed");
  }
  extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
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
  return std::strstr(extensions, ext) != nullptr;
#endif
}

} /* namespace gl */
} /* namespace radix */
