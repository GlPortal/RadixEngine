#ifndef RADIX_WINDOW_HPP
#define RADIX_WINDOW_HPP

#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL_video.h>

#include <radix/env/Config.hpp>
#include <radix/input/InputSource.hpp>
#include <radix/Viewport.hpp>
#include <radix/core/math/Vector2i.hpp>
#include <radix/core/math/Vector2f.hpp>

namespace radix {

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
  void create(const char *title);

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

  Config                             *config;       /**< System Configuration */

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
  void keyPressed(const KeyboardKey &key, const KeyboardModifier &mod) override;

  /**
   * @brief keyReleased handle released keyboard input
   * @param key input keyboard type
   * @param mod input keyboard modifier
   */
  void keyReleased(const KeyboardKey &key, const KeyboardModifier &mod) override;

  /**
   * @brief isKeyDown check keyboard status
   * @param key input keyboard type
   * @return if key is pressed
   */
  bool isKeyDown(const KeyboardKey &key) override;
  /**@} */

  /**
    * @name controller methods
    *@{ */
  /**
   * @brief controllerButtonPressed handle pressed controller button input
   * @param button input button type
   */
  void controllerButtonPressed(const ControllerButton &button, const ControllerIndex &index) override;

  /**
   * @brief controllerButtonPressed handle released controller button input
   * @param button input button type
   */
  void controllerButtonReleased(const ControllerButton &button, const ControllerIndex &index) override;

  /**
   * @brief controllerButtonPressed check controller button status
   * @param button input button type
   * @return if button is depressed
   */
  bool isControllerButtonDown(const ControllerButton &button, const ControllerIndex &index) override;

  /**
   * @brief getControllerAxisValue check controller axis value
   * @param axis axis index
   * @param index controller index
   */
  float getControllerAxisValue(const ControllerAxis &axis, const ControllerIndex &index) override;
  /**@} */

  /**
   * @brief controllerButtonPressed check mouse button status
   * @param button input button type
   * @return if button is depressed
   */
  bool isMouseButtonDown(const int &button) override;

  /**
   * @brief getMouseAxisValue get mouse axis value
   * @param axis x or y
   */
  Vector2f getRelativeMouseAxisValue() override;

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
  void printScreenToFile(const std::string &fileName);

private:
  /**
   * @brief flipVertical flip SDL surface vertically
   * @param sfc input SDL surface
   * @return flipped SDL surface
   */
  static SDL_Surface* flipVertical(SDL_Surface *sfc);

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

  void processMouseAxisEvents();

  void processControllerStickEvents();

  void processControllerTriggerEvents();

  /**
   * @brief processMouseButtonEvents get mouse event
   * and dispatch to subscribed listeners
   * @param event input mouse event
   */
  void processMouseButtonEvents(const SDL_Event &event);

  /**
   * @brief processWindowEvents get window event
   * and dispatch to subscribed listeners
   * @param event input window event
   */
  void processWindowEvents(const SDL_Event &event);

  /**
  * @brief  getOpenGlVersionString Create OpenGL version string
  *
  * @param _glVersion compact OpenGL version
  *
  * @return generate OpenGL string as "3.1" format
  */
  static std::string getOpenGlVersionString(const int _glVersion);

  /**
   * @brief initGl Initialize OpenGL 3.2 Core
   */
  void initGl();


  unsigned int  width;                          /**< main screen width */
  unsigned int  height;                         /**< main screen height */
  SDL_Window   *window;                         /**< SDL identifier for current window */
  SDL_GLContext context;                        /**< SDL Handler for OpenGL Context */

  SDL_Joystick *joystick;
  SDL_GameController *controller;

  std::vector<bool> mouseButtonStates;          /**< Mouse button pressed status */
  std::vector<bool> keyStates;                  /**< Keyboard key pressed status */
  std::vector<bool> controllerButtonStates;     /**< Controller button pressed status */
  std::vector<Vector2i> controllerStickStates;
  std::vector<Vector2i> controllerStickMax;
  std::vector<Vector2i> controllerStickMin;
  std::vector<int> controllerTriggerStates;
  std::string       charbuffer;                 /**< Text input buffer */
  bool lastNonZero;

  static const char        *DEFAULT_TITLE;      /**< Default Title Name */
  static const unsigned int DEFAULT_WIDTH;      /**< Default Window width */
  static const unsigned int DEFAULT_HEIGHT;     /**< Default Window Height */
};

} /* namespace radix */

#endif /* RADIX_WINDOW_HPP */
