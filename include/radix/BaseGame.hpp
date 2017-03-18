#ifndef RADIX_BASEGAME_HPP
#define RADIX_BASEGAME_HPP

#include <utility>
#include <functional>

#include <radix/util/sdl/Fps.hpp>
#include <radix/World.hpp>
#include <radix/GameWorld.hpp>
#include <radix/Window.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/renderer/ScreenRenderer.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/env/Config.hpp>
#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/data/map/CustomTrigger.hpp>
#include <radix/data/screen/Screen.hpp>
#include <radix/renderer/ScreenRenderer.hpp>

namespace radix {

const int UPDATE_RATE = 60;
const int SKIP_TIME = 1000 / UPDATE_RATE;
const int MAX_SKIP = 5;

class BaseGame {
public:
  static Fps fps;

  BaseGame();
  virtual ~BaseGame();

  // No copy
  BaseGame(const BaseGame&) = delete;
  BaseGame operator=(const BaseGame&) = delete;

  // No movement
  BaseGame(BaseGame&&) = delete;
  BaseGame operator=(BaseGame&&) = delete;

  bool isRunning();
  virtual void processInput();
  virtual void update();
  virtual void cleanUp();
  virtual void close();
  virtual void render();
  void setup();
  World* getWorld();
  ScreenRenderer* getScreenRenderer();
  GameWorld* getGameWorld();
  inline Window& getWindow() {
    return window;
  }
  inline std::list<CustomTrigger>& getCustomTriggers() {
    return customTriggers;
  };

protected:
  void loadMap();
  virtual void prepareCamera();
  virtual void initHook();
  virtual void customTriggerHook();

  Window window;
  World world;
  Config config;
  GameWorld gameWorld;

  std::shared_ptr<ScreenRenderer> screenRenderer;
  std::unique_ptr<Renderer> renderer;
  std::shared_ptr<radix::Screen> screen;
  EventDispatcher::CallbackHolder screenshotCallbackHolder;

  std::string defaultMap  = "/maps/n1.xml";
  std::string windowTitle = "RadixEngine";

  std::list<CustomTrigger> customTriggers;

  bool closed;
  unsigned int currentTime = 0, nextUpdate = 0, lastUpdate = 0, lastRender = 0;
private:
  void createWindow();
  void createScreenshotCallbackHolder();
};

} /* namespace radix */

#endif /* RADIX_BASEGAME_HPP */
