#include <radix/Window.hpp>

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <cstdlib>

#include <radix/core/gl/OpenGL.hpp>

#include <radix/core/diag/Throwables.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/gl/DebugOutput.hpp>
#include <radix/data/texture/TextureLoader.hpp>
#include <radix/env/Config.hpp>
#include <radix/env/Util.hpp>
#include <radix/input/GamePad.hpp>

#include <SDL.h>

namespace radix {

const unsigned int Window::DEFAULT_WIDTH = 800;
const unsigned int Window::DEFAULT_HEIGHT = 600;
const char *Window::DEFAULT_TITLE = "Radix Engine";

Window::Window() :
  width(0),
  height(0),
  window(nullptr),
  gamePad(),
  gamePadEnabled(true),
  ignoreGlVersion(false),
  antialiasLevel(0),
  mouseButtonStates((int)MouseButton::Max),
  keyStates(SDL_NUM_SCANCODES),
  controllerButtonStates(SDL_CONTROLLER_BUTTON_MAX),
  controllerStickStates(2),
  controllerStickMax(2, Vector2i(25000)),
  controllerStickMin(2, Vector2i(-25000)),
  controllerTriggerStates(2){}

Window::~Window() = default;

void Window::setConfig(radix::Config &config){
  this->config = &config;
}

void Window::setGamePadEnabled(bool enabled){
    this->gamePadEnabled = enabled;
}

void Window::setIgnoreGlVersion(bool enabled){
  this->ignoreGlVersion = enabled;
}

void Window::setAntialiasLevel(unsigned int value){
  this->antialiasLevel = value;
}

inline std::string Window::getOpenGlVersionString(const int glVersion) {
  const int majorGlVersion = glVersion / 10;
  const int minorGlVersion = glVersion % 10;

  std::string result;
  result =  majorGlVersion + "." + minorGlVersion;
  return std::string(result);
}

void Window::initGl() {
  gl::OpenGL::initialize();
  const int glversion = gl::OpenGL::version();
  const std::string versionString = getOpenGlVersionString(glversion);

  Util::Log(Verbose, "Window") << "OpenGL " << versionString;
  if (ignoreGlVersion) {
    Util::Log(Warning, "Window") << "Ignore OpenGl version";
  } else {
    if (glversion < 32) {
      throw Exception::Error("Window", std::string("OpenGL Version ") + versionString +
                             " is unsupported, " "required minimum is 3.2");
    }
  }
}

void Window::create(const char *title) {
  Uint32 controllerInitFlags = SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;
  Uint32 initFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
  if (gamePadEnabled) {
    initFlags |= controllerInitFlags;
  }

  if (SDL_Init(initFlags)  != 0) {
    throw Exception::Error("Window", std::string("Error on SDL init ") + SDL_GetError());
  }

  if (gamePadEnabled) {
    gamePad.create();
  }

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  if (antialiasLevel > 0) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, antialiasLevel);
  }

  int glFlags = 0;

  const bool enableGlDebug = config->isLoaded() && config->getGlContextEnableDebug();
  if (enableGlDebug) {
    glFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glFlags);

  int flags = SDL_WINDOW_OPENGL;

  Vector2i windowDimensions = getWindowDimensions();
  width  = windowDimensions.width;
  height = windowDimensions.height;

  if (config->isLoaded() && config->isFullscreen()) {
    flags |= SDL_WINDOW_BORDERLESS;
  } else {
    flags |= SDL_WINDOW_RESIZABLE;
    flags |= SDL_WINDOW_MAXIMIZED;
    // window starts maximized, so this will be the width/height after "unmaximizing"
    width  *= 0.90f;
    height *= 0.90f;
  }

  setSdlGlAttributes();

  // get the screen offest
  SDL_Rect rect;
  rect.x=SDL_WINDOWPOS_CENTERED;
  rect.y=SDL_WINDOWPOS_CENTERED;
  if(config->getScreen()!=0) {
      SDL_GetDisplayBounds(config->getScreen(), &rect);
  }
  window = SDL_CreateWindow(title, rect.x, rect.y, width, height, flags);

  context = SDL_GL_CreateContext(window);

  initGl();

  if (enableGlDebug) {
    gl::DebugOutput::enable();
  }

  glViewport(0, 0, width, height);

  // Allows unbound framerate if vsync is disabled
  if (config->isLoaded()) {
    SDL_GL_SetSwapInterval(config->hasVsync() ? 1 : 0);
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
  SDL_DisplayMode displayMode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
  SDL_GetDesktopDisplayMode(config->getScreen(), &displayMode);

  unsigned int widthConfig = 0, heightConfig = 0;
  if (config->isLoaded()) {
    widthConfig  = config->getWidth();
    heightConfig = config->getHeight();
  }

  width  = (widthConfig == 0) ? displayMode.w : widthConfig;
  height = (heightConfig == 0) ? displayMode.h : heightConfig;

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
  SDL_GameControllerClose(&gamePad.getController());

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


  processMouseAxisEvents();

  if (gamePadEnabled) {
    processControllerStickEvents();
    processControllerTriggerEvents();
  }

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
      const ControllerAddedEvent controllerAddedEvent(*this, event.cdevice.which);
      for (auto &dispatcher : dispatchers) {
        dispatcher.get().dispatch(controllerAddedEvent);
      }
      break;
    }
    case SDL_CONTROLLERDEVICEREMOVED: {
      const ControllerRemovedEvent controllerRemovedEvent(*this, event.cdevice.which);
      for (auto &dispatcher : dispatchers) {
        dispatcher.get().dispatch(controllerRemovedEvent);
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
    default: {
      break;
    }
    }
  }
}

void Window::processMouseAxisEvents() {
  Vector2i mouseRelative;
  SDL_GetRelativeMouseState(&mouseRelative.x, &mouseRelative.y);

  bool nonZero = mouseRelative != Vector2i::ZERO;

  if (nonZero || lastNonZero) {
    const MouseAxisEvent mae(*this, Vector2f(mouseRelative));
    for (auto &d : dispatchers) {
      d.get().dispatch(mae);
    }
  }

  lastNonZero = nonZero;
}

void Window::processControllerStickEvents() {
  for (int i = 0; i < 2; ++i) {
    Vector2i currentStickState;
    currentStickState.x = SDL_GameControllerGetAxis(&gamePad.getController(),
                                                    (SDL_GameControllerAxis)(2*i));
    currentStickState.y = SDL_GameControllerGetAxis(&gamePad.getController(),
                                                    (SDL_GameControllerAxis)(2*i + 1));

    if (currentStickState.x > controllerStickMax[i].x) {
      controllerStickMax[i].x = currentStickState.x;
    } else if (currentStickState.x < controllerStickMin[i].x) {
      controllerStickMin[i].x = currentStickState.x;
    }
    if (currentStickState.y > controllerStickMax[i].y) {
      controllerStickMax[i].y = currentStickState.y;
    } else if (currentStickState.y < controllerStickMin[i].y) {
      controllerStickMin[i].y = currentStickState.y;
    }

    Vector2i stickDelta = currentStickState - controllerStickStates[i];

    if (stickDelta != Vector2i::ZERO) {
      Vector2f normalisedStickState;
      if (currentStickState.x > 0) {
        normalisedStickState.x = float(currentStickState.x) / float(controllerStickMax[i].x);
      } else {
        normalisedStickState.x = float(currentStickState.x) / float(controllerStickMax[i].x);
      }

      if (currentStickState.y > 0) {
        normalisedStickState.y = float(currentStickState.y) / float(controllerStickMax[i].y);
      } else {
        normalisedStickState.y = float(currentStickState.y) / float(controllerStickMax[i].y);
      }

      const ControllerAxisEvent cae(*this, i, normalisedStickState, 0);
      for (auto &d : dispatchers) {
        d.get().dispatch(cae);
      }
    }

    controllerStickStates[i] = currentStickState;
  }
}

void Window::processControllerTriggerEvents() {
  for (int i = 0; i < 2; ++i) {
    int currentTriggerState = SDL_GameControllerGetAxis(&gamePad.getController(),
                                                        (SDL_GameControllerAxis)(i+4));

    int triggerDelta = currentTriggerState - controllerTriggerStates[i];

    if (triggerDelta) {
      float normalisedTriggerState = float(currentTriggerState) / 32767.0f;

      const ControllerTriggerEvent controllerTriggerEvent(*this, i, normalisedTriggerState, 0);
      for (auto &dispatcher : dispatchers) {
        dispatcher.get().dispatch(controllerTriggerEvent);
      }
    }

    controllerTriggerStates[i] = currentTriggerState;
  }
}

void Window::processMouseButtonEvents(const SDL_Event &event) {
  MouseButton button;

  switch (event.button.button) {
    case SDL_BUTTON_LEFT: {
      button = MouseButton::Left;
      break;
    }
    case SDL_BUTTON_MIDDLE: {
      button = MouseButton::Middle;
      break;
    }
    case SDL_BUTTON_RIGHT: {
      button = MouseButton::Right;
      break;
    }
    case SDL_BUTTON_X1: {
      button = MouseButton::Aux1;
      break;
    }
    case SDL_BUTTON_X2: {
      button = MouseButton::Aux2;
      break;
    }
    default: {
      button = MouseButton::Invalid;
      break;
    }
  }

  // Dispatch mouse event to subscribed listeners
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    mouseButtonStates[(int)button] = true;
    const MouseButtonPressedEvent mbpe(*this, button);
    for (auto &d : dispatchers) {
      d.get().dispatch(mbpe);
    }
  } else {
    mouseButtonStates[(int)button] = false;
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
  keyStates[key] = true;
  const KeyPressedEvent kpe(*this, key, mod);
  for (auto &d : dispatchers) {
    d.get().dispatch(kpe);
  }
}

void Window::keyReleased(const KeyboardKey &key, const KeyboardModifier &mod) {
  keyStates[key] = false;
  const KeyReleasedEvent kre(*this, key, mod);
  for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
    d.get().dispatch(kre);
  }
}

bool Window::isKeyDown(const KeyboardKey &key) {
  return keyStates[key];
}

// controller index is unused as of now, only the the first controller added is checked
void Window::controllerButtonPressed(const ControllerButton &button, const ControllerIndex &index) {
  this->controllerButtonStates[button] = true;
  const ControllerButtonPressedEvent cbpe(*this, button, index);
  for (auto &d : dispatchers) {
    d.get().dispatch(cbpe);
  }
}

void Window::controllerButtonReleased(const ControllerButton &button, const ControllerIndex &index) {
  this->controllerButtonStates[button] = false;
  const ControllerButtonReleasedEvent cbre(*this, button, index);
  for (auto &d : dispatchers) {
    d.get().dispatch(cbre);
  }
}

bool Window::isControllerButtonDown(const ControllerButton &button, const ControllerIndex &index) {
  return this->controllerButtonStates[button];
}

float Window::getControllerAxisValue(const ControllerAxis &axis, const ControllerIndex &index) {
  return float(SDL_GameControllerGetAxis(&gamePad.getController(), (SDL_GameControllerAxis)axis)) / 32767.0f;
}

bool Window::isMouseButtonDown(const int &button) {
  return this->mouseButtonStates[button];
}

Vector2f Window::getRelativeMouseAxisValue() {
  Vector2i mouseRelative;
  SDL_GetRelativeMouseState(&mouseRelative.x, &mouseRelative.y);

  return Vector2f(mouseRelative);
}

std::string Window::getCharBuffer() {
  return charbuffer;
}

void Window::addToBuffer(const std::string &character) {
  charbuffer.append(character);
}

void Window::clearBuffer() {
  charbuffer.clear();
}

void Window::truncateCharBuffer() {
  charbuffer = charbuffer.substr(0, charbuffer.size() - 1);
}

void Window::clear() {
  keyStates.clear();
}

void Window::printScreenToFile(const std::string &fileName) {
  Util::Log(Verbose, "Window") << "Taking screenshot";
  SDL_Surface *image =
    SDL_CreateRGBSurface(SDL_SWSURFACE,
                         width, height,
                         24, 0x000000FF,
                         0x0000FF00,
                         0x00FF0000, 0);

  // Read current OpenGL buffer to SDL_Surface
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  SDL_Surface *flippedImage = flipVertical(image);
  SDL_SaveBMP(flippedImage, fileName.c_str());
  // free image resources
  SDL_FreeSurface(flippedImage);
  SDL_FreeSurface(image);
}

SDL_Surface* Window::flipVertical(SDL_Surface *sfc) {
  SDL_Surface *result =
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

