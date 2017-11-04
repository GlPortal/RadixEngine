#include <radix/Window.hpp>

#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include <radix/core/gl/OpenGL.hpp>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/gl/DebugOutput.hpp>
#include <radix/data/texture/TextureLoader.hpp>
#include <radix/core/diag/Throwables.hpp>
#include <radix/env/Environment.hpp>
#include <radix/env/Util.hpp>

#include <SDL2/SDL.h>

namespace radix {

class Config;

const unsigned int Window::DEFAULT_WIDTH = 800;
const unsigned int Window::DEFAULT_HEIGHT = 600;
const char* Window::DEFAULT_TITLE = "Radix Engine";

Window::Window() :
  width(0),
  height(0),
  window(nullptr),
  controller(NULL),
  joystick(NULL),
  keystates(SDL_NUM_SCANCODES),
  controllerbuttonstates(SDL_CONTROLLER_BUTTON_MAX),
  mousebuttonstates(10) {}

Window::~Window() = default;

void Window::setConfig(radix::Config &config){
  this->config = config;
}

inline std::string Window::getOpenGlVersionString(const int _glVersion) {
  const int glmajor = _glVersion / 10;
  const int glminor = _glVersion % 10;

  char versionString[8];
  sprintf(versionString, "%d.%d", glmajor, glminor);

  return std::string(versionString);
}

void Window::initGl() {
  gl::OpenGL::initialize();
  const int glversion = gl::OpenGL::version();
  const std::string versionString = getOpenGlVersionString(glversion);

  Util::Log(Verbose, "Window") << "OpenGL " << versionString;
  if (config.getIgnoreGlVersion()) {
    Util::Log(Warning, "Window") << "Ignore OpenGl version";
  } else {
    if (glversion < 32) {
      throw Exception::Error("Window", std::string("OpenGL Version ") + versionString +
                             " is unsupported, required minimum is 3.2");
    }
  }
}

void Window::create(const char *title) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

  int numJoysticks = SDL_NumJoysticks();
  Util::Log(Verbose, "Window") << "Number of joysticks " << std::to_string(numJoysticks);

  for (int i = 0; i < numJoysticks; ++i) {
    if (SDL_IsGameController(i)) {
      controller = SDL_GameControllerOpen(i);
      if (controller) {
        Util::Log(Warning, "Window") << "Controller of index " << i << " loaded";

        joystick = SDL_JoystickOpen(i);

        break;
      } else {
        Util::Log(Warning, "Window") << "Controller of index " << i << " unable to load";
      }
    } else {
      Util::Log(Warning, "Window") << "Joystick of index " << i << " not a controller";
    }
  }

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  if (config.isLoaded() && config.getAntialiasLevel() > 0) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config.getAntialiasLevel());
  }

  int glFlags = 0;

  const bool enableGlDebug = config.isLoaded() && config.getGlContextEnableDebug();
  if (enableGlDebug) {
    glFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glFlags);

  int flags = SDL_WINDOW_OPENGL;

  if (config.isLoaded() && config.isFullscreen()) {
    flags |= SDL_WINDOW_BORDERLESS;
  }

  Vector2i windowDimensions = getWindowDimensions();
  width  = windowDimensions.width;
  height = windowDimensions.height;

  setSdlGlAttributes();

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, flags);

  context = SDL_GL_CreateContext(window);

  initGl();

  if (enableGlDebug) {
    gl::DebugOutput::enable();
  }

  glViewport(0, 0, width, height);

  // Allows unbound framerate if vsync is disabled
  if (config.isLoaded()) {
    SDL_GL_SetSwapInterval(config.hasVsync() ? 1 : 0);
  } else {
    SDL_GL_SetSwapInterval(1);
  }

  // Lock cursor in the middle of the screen
  lockMouse();
}

Vector2i Window::getWindowDimensions() {
  /* Get screen 0 info (main screen)
   * - pixel format
   * - screen width
   * - screen height
   */
  SDL_DisplayMode dispMode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
  SDL_GetDesktopDisplayMode(0, &dispMode);

  unsigned int widthConfig = 0, heightConfig = 0;
  if (config.isLoaded()) {
    widthConfig  = config.getWidth();
    heightConfig = config.getHeight();
  }

  width  = (widthConfig == 0) ? dispMode.w : widthConfig;
  height = (heightConfig == 0) ? dispMode.h : heightConfig;

  return Vector2i(width, height);
}

void Window::setFullscreen() {
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void Window::swapBuffers() {
  SDL_GL_SwapWindow(window);
}

void Window::getSize(int *width, int *height) const {
  SDL_GetWindowSize(window, width, height);
}

void Window::close() {
  SDL_GameControllerClose( controller );
  controller = NULL;

  SDL_HideWindow(window);

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  window = nullptr;

  SDL_Quit();
}

void Window::lockMouse() {
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::unlockMouse() {
  SDL_WarpMouseInWindow(window, width / 2, height / 2);
  SDL_SetRelativeMouseMode(SDL_FALSE);
}

void Window::processEvents() {
  SDL_Event event;

  // SDL only creates mouse motion events if the relative motion is above some threshhold which is pretty high
  // so the relative movement must be acquired manually

  SDL_GetRelativeMouseState(&mouse_xrel, &mouse_yrel);

  while (SDL_PollEvent(&event)) {
    int key = event.key.keysym.scancode;
    int mod = event.key.keysym.mod;
    int sym = event.key.keysym.sym;
    // int cursor = event.edit.start;
    // int selection_len = event.edit.length;

    switch (event.type) {
    case SDL_TEXTINPUT: {
      addToBuffer(event.text.text);
      break;
    }
    case SDL_TEXTEDITING: {
      break;
    }
    case SDL_KEYDOWN: {
      if (sym == SDLK_BACKSPACE) {
        truncateCharBuffer();
      }

      if (sym == SDLK_RETURN) {
        clearBuffer();
      }

      keyPressed(key, mod);
      break;
    }
    case SDL_KEYUP: {
      keyReleased(key, mod);
      break;
    }
    case SDL_CONTROLLERBUTTONDOWN: {
      controllerButtonPressed(event.cbutton.button, event.cbutton.which);
      break;
    }
    case SDL_CONTROLLERBUTTONUP: {
      controllerButtonReleased(event.cbutton.button, event.cbutton.which);
      break;
    }
    case SDL_CONTROLLERDEVICEADDED: {
      const ControllerAddedEvent cae(*this, event.cdevice.which);
      for (auto &d : dispatchers) {
        d.get().dispatch(cae);
      }
      break;
    }
    case SDL_CONTROLLERDEVICEREMOVED: {
      const ControllerRemovedEvent cre(*this, event.cdevice.which);
      for (auto &d : dispatchers) {
        d.get().dispatch(cre);
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      const MouseMovedEvent mre(*this, mouse_xrel, mouse_yrel);
      for (auto &d : dispatchers) {
        d.get().dispatch(mre);
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP: {
      processMouseButtonEvents(event);
      break;
    }
    case SDL_MOUSEWHEEL: {
      const int dirmult = (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ? -1 : 1;
      const MouseWheelScrolledEvent mwse(*this, event.wheel.x * dirmult, event.wheel.y * dirmult);
      for (auto &d : dispatchers) {
        d.get().dispatch(mwse);
      }
      break;
    }
    case SDL_WINDOWEVENT: {
      processWindowEvents(event);
      break;
    }
    }
  }
}

void Window::processMouseButtonEvents(const SDL_Event &event) {
  MouseButton button;

  switch (event.button.button) {
    case SDL_BUTTON_LEFT: {
      button = MouseButton::MOUSE_BUTTON_LEFT;
      break;
    }
    case SDL_BUTTON_MIDDLE: {
      button = MouseButton::MOUSE_BUTTON_MIDDLE;
      break;
    }
    case SDL_BUTTON_RIGHT: {
      button = MouseButton::MOUSE_BUTTON_RIGHT;
      break;
    }
    case SDL_BUTTON_X1: {
      button = MouseButton::MOUSE_BUTTON_AUX1;
      break;
    }
    case SDL_BUTTON_X2: {
      button = MouseButton::MOUSE_BUTTON_AUX2;
      break;
    }
    default: {
      button = MouseButton::MOUSE_BUTTON_INVALID;
      break;
    }
  }

  // Dispatch mouse event to subscribed listeners
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    mousebuttonstates[int(button)] = true;
    const MouseButtonPressedEvent mbpe(*this, button);
    for (auto &d : dispatchers) {
      d.get().dispatch(mbpe);
    }
  } else {
    mousebuttonstates[int(button)] = false;
    const MouseButtonReleasedEvent mbre(*this, button);
    for (auto &d : dispatchers) {
      d.get().dispatch(mbre);
    }
  }
}

void Window::processWindowEvents(const SDL_Event &event) {
  switch (event.window.event) {
    case SDL_WINDOWEVENT_SHOWN: {
      const WindowShownEvent wse(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wse);
      }
      break;
    }
    case SDL_WINDOWEVENT_HIDDEN: {
      const WindowHiddenEvent whe(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(whe);
      }
      break;
    }
    case SDL_WINDOWEVENT_EXPOSED: {
      const WindowExposedEvent wee(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wee);
      }
      break;
    }
    case SDL_WINDOWEVENT_MOVED: {
      const WindowMovedEvent wme(*this, event.window.windowID, event.window.data1, event.window.data2);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wme);
      }
      break;
    }
    case SDL_WINDOWEVENT_RESIZED: {
      const WindowResizedEvent wre(*this, event.window.windowID, event.window.data1, event.window.data2);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wre);
      }
      break;
    }
    case SDL_WINDOWEVENT_SIZE_CHANGED: {
      const WindowSizeChangedEvent wsce(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wsce);
      }
      break;
    }
    case SDL_WINDOWEVENT_MINIMIZED: {
      const WindowMinimizedEvent wmie(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wmie);
      }
      break;
    }
    case SDL_WINDOWEVENT_MAXIMIZED: {
      const WindowMaximizedEvent wmae(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wmae);
      }
      break;
    }
    case SDL_WINDOWEVENT_RESTORED: {
      const WindowRestoredEvent wree(*this, event.window.windowID); //REEEE
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wree);
      }
      break;
    }
    case SDL_WINDOWEVENT_ENTER: {
      const WindowEnterEvent wene(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wene);
      }
      break;
    }
    case SDL_WINDOWEVENT_LEAVE: {
      const WindowLeaveEvent wle(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wle);
      }
      break;
    }
    case SDL_WINDOWEVENT_FOCUS_GAINED: {
      const WindowFocusGainedEvent wfge(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wfge);
      }
      break;
    }
    case SDL_WINDOWEVENT_FOCUS_LOST: {
      const WindowFocusLostEvent wfle(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wfle);
      }
      break;
    }
    case SDL_WINDOWEVENT_CLOSE: {
      const WindowCloseEvent wce(*this, event.window.windowID);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
        d.get().dispatch(wce);
      }
      break;
    }
    default: {
      Util::Log(Verbose, "Window") << "Unknown window event type!";
      break;
    }
  }
}

void Window::keyPressed(const KeyboardKey &key, const KeyboardModifier &mod) {
  keystates[key] = true;
  const KeyPressedEvent kpe(*this, key, mod);
  for (auto &d : dispatchers) {
    d.get().dispatch(kpe);
  }
}

void Window::keyReleased(const KeyboardKey &key, const KeyboardModifier &mod) {
  keystates[key] = false;
  const KeyReleasedEvent kre(*this, key, mod);
  for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
    d.get().dispatch(kre);
  }
}

bool Window::isKeyDown(const KeyboardKey &key) {
  return keystates[key];
}

// controller index is unused as of now as I only check the the first controller added
void Window::controllerButtonPressed(const ControllerButton &button, const ControllerIndex &index) {
  this->controllerbuttonstates[button] = true;
  const ControllerButtonPressedEvent cbpe(*this, button, index);
  for (auto &d : dispatchers) {
    d.get().dispatch(cbpe);
  }
}

void Window::controllerButtonReleased(const ControllerButton &button, const ControllerIndex &index) {
  this->controllerbuttonstates[button] = false;
  const ControllerButtonReleasedEvent cbre(*this, button, index);
  for (auto &d : dispatchers) {
    d.get().dispatch(cbre);
  }
}

bool Window::isControllerButtonDown(const ControllerButton &button, const ControllerIndex &index) {
  return this->controllerbuttonstates[button];
}

int Window::getControllerAxisValue(const ControllerAxis &axis, const ControllerIndex &index) {
  return SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)axis);
}

bool Window::isMouseButtonDown(const int &button) {
  return this->mousebuttonstates[button];
}

int Window::getRelativeMouseAxisValue(const int &axis) {
  if (axis == int(MouseAxis::MOUSE_AXIS_X)) {
    return mouse_xrel;
  } else if (axis == int(MouseAxis::MOUSE_AXIS_Y)) {
    return mouse_yrel;
  } else {
    return 0;
  }
}

void Window::getRelativeMouseState(int *dx, int *dy) {
  SDL_GetRelativeMouseState(dx, dy);
}

std::string Window::getCharBuffer() {
  return charbuffer;
}

void Window::addToBuffer(const std::string& character) {
  charbuffer.append(character);
}

void Window::clearBuffer() {
  charbuffer.clear();
}

void Window::truncateCharBuffer() {
  charbuffer = charbuffer.substr(0, charbuffer.size() - 1);
}

void Window::clear() {
  keystates.clear();
}

void Window::printScreenToFile(const std::string& fileName) {
  Util::Log(Verbose, "Window") << "Taking screenshot";
  SDL_Surface * image =
    SDL_CreateRGBSurface(SDL_SWSURFACE,
                         width, height,
                         24, 0x000000FF,
                         0x0000FF00,
                         0x00FF0000, 0);

  // Read current OpenGL buffer to SDL_Surface
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  SDL_Surface * flippedImage = flipVertical(image);
  SDL_SaveBMP(flippedImage, fileName.c_str());
  // free image resources
  SDL_FreeSurface(flippedImage);
  SDL_FreeSurface(image);
}

SDL_Surface* Window::flipVertical(SDL_Surface* sfc) {
  SDL_Surface* result =
    SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
                         sfc->format->BytesPerPixel * 8,
                         sfc->format->Rmask, sfc->format->Gmask,
                         sfc->format->Bmask, sfc->format->Amask);

  const auto pitch = sfc->pitch;
  const auto pxlength = pitch*sfc->h;
  auto pixels = static_cast<unsigned char*>(sfc->pixels) + pxlength;
  auto rpixels = static_cast<unsigned char*>(result->pixels);

  for (auto line = 0; line < sfc->h; ++line) {
    memcpy(rpixels, pixels, pitch);
    pixels -= pitch;
    rpixels += pitch;
  }

  return result;
}

void Window::setSdlGlAttributes() {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

} /* namespace radix */

