#ifndef RADIX_BASEGAME_HPP
#define RADIX_BASEGAME_HPP

#include <functional>
#include <memory>
#include <utility>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/data/map/CustomTrigger.hpp>
#include <radix/env/Config.hpp>
#include <radix/util/sdl/Fps.hpp>
#include <radix/GameWorld.hpp>
#include <radix/Window.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/renderer/ScreenRenderer.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/data/map/XmlMapLoader.hpp>
#include <radix/data/map/CustomTrigger.hpp>
#include <radix/data/screen/Screen.hpp>
#include <radix/renderer/ScreenRenderer.hpp>
#include <radix/env/Config.hpp>
#include <radix/input/InputManager.hpp>
#include <radix/api/ScriptEngine.hpp>

namespace radix {

class Renderer;
class Screen;
class ScreenRenderer;
class World;

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

  void setup();
  virtual void postSetup() {}

  virtual void onPreCreateWorld(World &world) {}
  virtual void onPostCreateWorld(World &world) {}
  virtual void onPreStartWorld() {}
  virtual void onPostStartWorld() {}
  virtual void onPreStopWorld() {}
  virtual void onPostStopWorld() {}
  virtual void onPreDestroyWorld(World &world) {}
  virtual void onPostDestroyWorld(World &world) {}

  bool isRunning();
  void preCycle();
  virtual void processInput();
  virtual void update();
  virtual void cleanUp();
  virtual void close();
  virtual void render();
  void postCycle();

  void deferPostCycle(const std::function<void()>&);

  World* getWorld();
  Config& getConfig();
  inline InputManager* getInputManager() {
    return inputManager.get();
  }

  template<class T, typename... Args>
  T& createOtherWorld(const std::string &name, Args&&... args) {
    auto r = otherWorlds.emplace(std::piecewise_construct,
        std::forward_as_tuple(name),
        std::forward_as_tuple(new T(*this, std::forward(args)...)));
    createWorld(*r.first->second);
    return static_cast<T&>(*r.first->second);
  }
  World& getOtherWorld(const std::string &name) const {
    return *otherWorlds.at(name);
  }
  void switchToOtherWorld(const std::string &name);
  void clearOtherWorldList();
  ScreenRenderer* getScreenRenderer();
  GameWorld* getGameWorld();
  ScriptEngine* getScriptEngine();
  inline Window& getWindow() {
    return window;
  }
  inline std::list<CustomTrigger>& getCustomTriggers() {
    return customTriggers;
  }

protected:
  void setWorld(std::unique_ptr<World>&&);
  void createWorld(World&);

  void loadMap(World&);
  virtual void prepareCamera();
  virtual void onInit();
  virtual void onShutdown();
  virtual void customTriggerHook();

  Window window;
  std::unique_ptr<InputManager> inputManager;
  std::map<std::string, std::unique_ptr<World>> otherWorlds;
  std::unique_ptr<World> world;
  Config config;
  GameWorld gameWorld;
  std::unique_ptr<ScriptEngine> scriptEngine;
  std::vector<std::function<void()>> postCycleDeferred;

  std::shared_ptr<ScreenRenderer> screenRenderer;
  std::unique_ptr<Renderer> renderer;
  std::shared_ptr<radix::Screen> screen;
  EventDispatcher::CallbackHolder screenshotCallbackHolder;

  std::string defaultMap;
  std::string windowTitle;

  std::list<CustomTrigger> customTriggers;

  bool closed;
  unsigned int currentTime = 0, lastUpdate = 0, lastRender = 0;
private:
  void createWindow();
  void createScreenshotCallbackHolder();
};

} /* namespace radix */

#endif /* RADIX_BASEGAME_HPP */
