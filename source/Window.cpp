#include <radix/Window.hpp>

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include <epoxy/gl.h>

#include <SDL2/SDL.h>

#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/CheckBox.h>
#include <Gwen/Controls/TextBox.h>
#include <Gwen/Controls/TreeControl.h>
#include <chrono>
#include <thread>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/texture/TextureLoader.hpp>
#include <radix/core/diag/Throwables.hpp>
#include <radix/input/GWENInput.hpp>
#include <radix/renderer/GlGwenRenderer.hpp>
#include <radix/env/Environment.hpp>
#include <radix/env/Util.hpp>

namespace radix {

const unsigned int Window::DEFAULT_WIDTH = 800;
const unsigned int Window::DEFAULT_HEIGHT = 600;
const char* Window::DEFAULT_TITLE = "Radix Engine";

Window::Window() :
  config(),
  width(0),
  height(0),
  window(nullptr),
  keystates(SDL_NUM_SCANCODES) {
}

Window::~Window() = default;

void Window::setConfig(radix::Config &config){
  this->config = config;
}

void Window::initEpoxy() {
  const int glver = epoxy_gl_version(), glmaj = glver / 10, glmin = glver % 10;
  const std::string verstr = std::to_string(glmaj) + '.' + std::to_string(glmin);
  Util::Log(Verbose, "Window") << "OpenGL " << verstr;
  if (config.getIgnoreGlVersion()) {
    Util::Log(Warning, "Window") << "Ignore OpenGl version";
  } else {
    if (glver < 32) {
      throw Exception::Error("Window", std::string("OpenGL Version ") + verstr + " is unsupported, "
        "required minimum is 3.2");
    }
  }
}

void Window::create(const char *title) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  if (config.isLoaded() && config.getAntialiasLevel() > 0) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config.getAntialiasLevel());
  }

  int flags = SDL_WINDOW_OPENGL;

  if (config.isLoaded() && config.isFullscreen()) {
    flags |= SDL_WINDOW_BORDERLESS;
  }

  SDL_DisplayMode dispMode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
  SDL_GetDesktopDisplayMode(0, &dispMode);

  if (config.isLoaded()) {
    unsigned int widthConfig, heightConfig;
    widthConfig  = config.getWidth();
    heightConfig = config.getHeight();

    width  = widthConfig;
    height = heightConfig;

    if (widthConfig == 0) {
      width = dispMode.w;
    }
    if (heightConfig == 0) {
      height = dispMode.h;
    }
  } else {
    width = dispMode.w;
    height = dispMode.h;
    Util::Log(Verbose, "Window") << "W  " << width;
     //exit (EXIT_FAILURE);
  }


  // Explicitly request an OpenGL 3.2 Core context
  // i.e. enforce using non-deprecated functions
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, flags);

  context = SDL_GL_CreateContext(window);

  initEpoxy();

  glViewport(0, 0, width, height);

  // Allows unbound framerate if vsync is disabled
  if (config.isLoaded()) {
    SDL_GL_SetSwapInterval(config.hasVsync() ? 1 : 0);
  } else {
    SDL_GL_SetSwapInterval(1);
  }

  // Lock cursor in the middle of the screen
  lockMouse();

  gwenRenderer = std::make_unique<GlGwenRenderer>();
  gwenSkin = std::make_unique<Gwen::Skin::TexturedBase>(gwenRenderer.get());
  gwenSkin->Init((Environment::getDataDir() + "/gui/DefaultSkin.png").c_str());
  gwenCanvas = std::make_unique<Gwen::Controls::Canvas>(gwenSkin.get());
  gwenInput = std::make_unique<GWENInput>();
  gwenRenderer->Init();
  gwenInput->init(gwenCanvas.get());

#if 0 // Testing code
  {
    using namespace Gwen::Controls;
    WindowControl *win = new WindowControl(gwenCanvas.get());
    win->SetTitle("Texture cache");
    win->SetBounds(30, 30, 500, 200);
    TreeControl *tree = new TreeControl(win, "tree");
    tree->SetBounds(0, 0, 200, 186);
    std::thread thr([tree]() {
      while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Base::List &l = tree->GetChildNodes();
        for (auto it : TextureLoader::getTextureCache()) {
          bool add = true;
          for (Base *b : l) {
            if (((TreeNode*)b)->GetName() == it.first) {
              add = false;
              break;
            }
          }
          if (add) {
            TreeNode *n = tree->AddNode(it.first);
            n->SetName(it.first);
          }
        }
      }
    });
    thr.detach();
  }
#endif
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
  SDL_HideWindow(window);

  gwenInput.release();
  gwenCanvas.release();
  gwenSkin.release();
  gwenRenderer.release();

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
    case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        processMouseButtonEvents(event);
        break;
    }
    case SDL_MOUSEWHEEL: {
      const int dirmult = (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ? -1 : 1;
      const MouseWheelScrolledEvent mwse(*this, event.wheel.x * dirmult, event.wheel.y * dirmult);
      for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
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

void Window::processMouseButtonEvents(SDL_Event &event) {
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
      button = MouseButton::Unknown;
      break;
    }
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    const MouseButtonPressedEvent mbpe(*this, button);
    for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
      d.get().dispatch(mbpe);
    }
  } else {
    const MouseButtonReleasedEvent mbre(*this, button);
    for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
      d.get().dispatch(mbre);
    }
  }
}

void Window::processWindowEvents(SDL_Event &event) {
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

void Window::keyPressed(KeyboardKey key, KeyboardModifier mod) {
  keystates[key] = true;
  const KeyPressedEvent kpe(*this, key, mod);
  for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
    d.get().dispatch(kpe);
  }
}

void Window::keyReleased(KeyboardKey key, KeyboardModifier mod) {
  keystates[key] = false;
  const KeyReleasedEvent kre(*this, key, mod);
  for (std::reference_wrapper<EventDispatcher> &d : dispatchers) {
    d.get().dispatch(kre);
  }
}

bool Window::isKeyDown(KeyboardKey key) {
  return keystates[key];
}

void Window::addToBuffer(const std::string &character) {
  charbuffer.append(character);
}

std::string Window::getCharBuffer() {
  return charbuffer;
}

void Window::clearBuffer() {
  charbuffer = "";
}

void Window::truncateCharBuffer() {
  if (charbuffer.size() > 0) {
    charbuffer = charbuffer.substr(0, charbuffer.size() - 1);
  }
}

void Window::clear() {
  keystates.clear();
}

void Window::printScreenToFile(std::string fileName) {
  Util::Log(Verbose, "Window") << "Taking screenshot";
  SDL_Surface * image =
    SDL_CreateRGBSurface(SDL_SWSURFACE,
                         width, height,
                         24, 0x000000FF,
                         0x0000FF00,
                         0x00FF0000, 0);

  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  SDL_Surface * flippedImage = flipVertical(image);
  SDL_SaveBMP(flippedImage, fileName.c_str());
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
} /* namespace radix */
