#include <radix/OpenGL.hpp>

#include <string>
#include <iostream>
#include <radix/core/diag/Throwables.hpp>
#include <radix/env/Util.hpp>
#include <radix/Window.hpp>

namespace radix {
    namespace opengl {
        void initGL() {
            #ifdef EPOXY_SUPPORT
                const int glver = epoxy_gl_version(), glmaj = glver / 10, glmin = glver % 10;
                const std::string versionString = std::to_string(glmaj) + '.' + std::to_string(glmin);
                Util::Log(Verbose, "Window") << "OpenGL " << versionString;

                if (glver < 32) {
                    throw Exception::Error("Window", std::string("OpenGL Version ") + versionString +
                        " is unsupported, " "required minimum is 3.2");
                }
            #elif GLAD_SUPPORT
                if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
                    Util::Log(Error, "Window") << "Failed to initialize OpenGL context";
                    const std::string versionString = std::to_string(GLVersion.major) + '.' + std::to_string(GLVersion.minor);
                    throw Exception::Error("Window", std::string("OpenGL Version ") + versionString +
                        " is unsupported, " "required minimum is 3.2");
                }

                Util::Log(Info, "Window") << "Loaded OpenGL version: " << GLVersion.major << "." << GLVersion.minor;
            #endif
        }
    }
}
