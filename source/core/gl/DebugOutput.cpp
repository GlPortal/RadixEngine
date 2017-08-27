#include <radix/core/gl/DebugOutput.hpp>

#include <radix/core/gl/OpenGL.hpp>

#include <radix/env/Util.hpp>

namespace radix {
namespace gl {

static void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id,
  GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
  const char *sourceStr = "???";
  switch (source) {
    case GL_DEBUG_SOURCE_API:
      sourceStr = "API";
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      sourceStr = "WIN";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      sourceStr = "SHC";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      sourceStr = "3PT";
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      sourceStr = "APP";
      break;
    case GL_DEBUG_SOURCE_OTHER:
      sourceStr = "OTH";
      break;
  }
  const char *typeStr = "???";
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      typeStr = "ERR";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      typeStr = "DEP";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      typeStr = "UND";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      typeStr = "PRT";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      typeStr = "PRF";
      break;
    case GL_DEBUG_TYPE_MARKER:
      typeStr = "MKR";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      typeStr = "GP+";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      typeStr = "GP-";
      break;
    case GL_DEBUG_TYPE_OTHER:
      typeStr = "OTH";
      break;
  }
  const char *severityStr = "???";
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    severityStr = "HIG";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    severityStr = "MED";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    severityStr = "LOW";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    severityStr = "NOT";
    break;
  }
  char line[3 + 3 + 1 + 3 + 1 + 3 + 1];
  std::snprintf(line, sizeof(line)/sizeof(line[0]), "GL %3s %3s %3s", sourceStr, typeStr, severityStr);
  if (message[length - 1] == '\n') {
    Util::Log(Debug, line) << stx::string_view(message, length - 2);
  } else {
    Util::Log(Debug, line) << message;
  }
}

void DebugOutput::enable() {
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  glDebugMessageCallback(glDebugCallback, nullptr);
}

void DebugOutput::disable() {
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
}

} /* namespace gl */
} /* namespace radix */
