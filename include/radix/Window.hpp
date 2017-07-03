#ifndef RADIX_WINDOW_HPP
#define RADIX_WINDOW_HPP

#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL_video.h>

#include <Gwen/Controls/Canvas.h>
#include <Gwen/Skins/TexturedBase.h>

#include <radix/input/InputSource.hpp>
#include <radix/Viewport.hpp>
#include <radix/env/Config.hpp>
#include <radix/core/math/Vector2i.hpp>

namespace radix {

class GlGwenRenderer;
class GWENInput;

class Window : public Viewport, public InputSource {
public:
  /**
   * @brief Window default constructor
   */
  Window();

  /**
   * @brief ~Window default destructor
   */
  ~Window() override;

  /**
   * @brief create initialize SDL and OpenGL
   * @param title window title
   */
  void create(const char* title);

  /**
   * @brief setFullscreen set Window FullScreen
   */
  void setFullscreen();

  /**
   * @brief setConfig set system configuration
   * @param config current configuration
   */
  void setConfig(radix::Config &config);

  /**
   * @brief swapBuffers swap OpenGL double buffer
   */
  void swapBuffers();

  /**
   * @brief getSize current active window width and height
   * @param width  current active window width
   * @param height current active window height
   */
  void getSize(int *width, int *height) const override;

  /**
   * @brief close ShutDown all subsystem
   */
  void close();

  /**
    * @name mouseVisibility
    *@{ */
  /**
   * @brief lockMouse hide mouse cursor
   */
  void lockMouse();

  /**
   * @brief unlockMouse set mouse to center of screen
   * unhide mouse cursor
   */
  void unlockMouse();
  /**@} */

  std::unique_ptr<radix::GlGwenRenderer>    gwenRenderer; /**< Rendering subsystem */
  std::unique_ptr<Gwen::Skin::TexturedBase> gwenSkin;     /**< Texture subsystem */
  std::unique_ptr<Gwen::Controls::Canvas>   gwenCanvas;   /**< Canvas subsystem */
  std::unique_ptr<radix::GWENInput>         gwenInput;    /**< Input subsystem */
  radix::Config                             config;       /**< System Configuration */

  /**
   * @brief processEvents main event loop handler
   */
  void processEvents() override;

  /**
    * @name keyboard methods
    *@{ */
  /**
   * @brief keyPressed handle pressed keyboard input
   * @param key input keyboard type
   * @param mod input keyboard mode
   */
  void keyPressed(KeyboardKey key, KeyboardModifier mod) override;

  /**
   * @brief keyReleased handle released keyboard input
   * @param key input keyboard type
   * @param mod input keyboard modifier
   */
  void keyReleased(KeyboardKey key, KeyboardModifier mod) override;

  /**
   * @brief isKeyDown check keyboard status
   * @param key input keyboard type
   * @return if key is pressed
   */
  bool isKeyDown(KeyboardKey key) override;
  /**@} */

  /**
   * @name CharBuffer methods
   * Manipulate input buffer
   *@{ */
  /**
   * @brief getCharBuffer get text input
   * @return get copy of CharBuffer
   */
  std::string getCharBuffer() override;

  /**
   * @brief addToBuffer add text input to buffer
   * @param character text input
   */
  void addToBuffer(const std::string &character) override;

  /**
   * @brief clearBuffer clear charBuffer
   */
  void clearBuffer() override;

  /**
   * @brief truncateCharBuffer shrink charBuffer to
   * fit current buffer size
   */
  void truncateCharBuffer() override;
  /**@} */

  /**
   * @brief clear clear input key status
   */
  void clear() override;

  /**
   * @brief printScreenToFile create snapshot from
   * screen and write it to hard as BMP
   * @param fileName image file name
   */
  void printScreenToFile(const std::string& fileName);

private:
  /**
   * @brief flipVertical flip SDL surface vertically
   * @param sfc input SDL surface
   * @return flipped SDL surface
   */
  static SDL_Surface* flipVertical(SDL_Surface* sfc);

  /**
   * @brief getWindowDimensions get main screen dimension
   * @return get width and height
   */
  Vector2i getWindowDimensions();

  /**
   * @brief setSdlGlAttributes Explicitly request
   * an OpenGL 3.2 Core context
   * i.e. enforce using non-deprecated functions
   */
  void setSdlGlAttributes();

  /**
   * @brief processMouseButtonEvents get mouse event
   * and dispatch to subscribed listeners
   * @param event input mouse event
   */
  void processMouseButtonEvents(SDL_Event &event);

  /**
   * @brief processWindowEvents get window event
   * and dispatch to subscribed listeners
   * @param event input window event
   */
  void processWindowEvents(SDL_Event &event);

  /** 
  * @brief  getOpenGlVersionString Create OpenGL version string
  * 
  * @param _glVersion compact OpenGL version
  * 
  * @return generate OpenGL string as "3.1" format
  */
  static std::string getOpenGlVersionString(const int _glVersion);

  /**
   * @brief initGl Initilize OpenGL 3.2 Core
   */
  void initGl();

  /**
   * @brief initGwen Initlize Gwen subsystem
   */
  void initGwen();

  /**
   * @brief testGwen
   */
  void testGwen();

  unsigned int  width;          /**< main screen width */
  unsigned int  height;         /**< main screen height */
  SDL_Window   *window;         /**< SDL identifier for current window */
  SDL_GLContext context;        /**< SDL Handler for OpenGL Context */

  std::vector<bool> keystates;  /**< Keyboard pressed key status */
  std::string       charbuffer; /**< Text input buffer */

  static const char*        DEFAULT_TITLE;  /**< Default Title Name */
  static const unsigned int DEFAULT_WIDTH;  /**< Default Window width */
  static const unsigned int DEFAULT_HEIGHT; /**< Default Window Height */
};

} /* namespace radix */

#endif /* RADIX_WINDOW_HPP */
