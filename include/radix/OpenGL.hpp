#ifndef RADIX_OPENGL_HPP
#define RADIX_OPENGL_HPP

#ifdef EPOXY_SUPPORT
#include <epoxy/gl.h>
#elif GLAD_SUPPORT
#include <glad/glad.h>
#endif

namespace radix {
    namespace opengl {
        void initGL();
        static bool uniformBufferSupport() {
#ifdef EPOXY_SUPPORT
            return epoxy_has_gl_extension("GL_ARB_uniform_buffer_object");
#elif GLAD_SUPPORT
            return !!GLAD_GL_ARB_uniform_buffer_object;
#else 
            return false;
#endif

        }
    }
}

#endif