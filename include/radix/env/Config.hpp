#ifndef RADIX_CONFIG_HPP
#define RADIX_CONFIG_HPP

#include <string>
#include <vector>
#include <map>

#include <json11/json11.hpp>

#include <radix/core/diag/Logger.hpp>
#include <radix/input/Bind.hpp>

using namespace json11;

namespace radix {

class ArgumentsParser;

/** @brief Configuration class
 *
 *  Load and represent the configuration
 */
class Config {
friend class ArgumentsParser;

public:
  using Bindings = std::vector<std::vector<Bind>>;

  Config();
  void load();
  bool isLoaded();
  unsigned int getWidth()         const { return width; }
  unsigned int getHeight()        const { return height; }
  float getMouseSensitivity()     const { return mouseSensitivity; }
  bool isFullscreen()             const { return fullscreen; }
  int getAntialiasLevel()         const { return antialiasing; }
  int getRecursionLevel()         const { return recursivePortal; }
  bool hasSound()                 const { return sound; }
  bool hasVsync()                 const { return vsync; }
  bool isHidePortalsByClick()     const { return hidePortalsByClick; }
  bool isConsoleEnabled()         const { return consoleEnabled; }
  bool isProfilerEnabled()        const { return profilerEnabled; }
  bool isFlyingEnabled()          const { return flyingEnabled; }
  bool isDebugViewEnabled()       const { return debugViewEnabled; }
  bool getCursorVisibility()      const { return cursorVisibility; }
  bool getIgnoreGlVersion()       const { return ignoreGlVersion; }
  bool getGlContextEnableDebug()  const { return glContextEnableDebug; }
  Bindings getBindings()          const { return bindings; }
  LogLevel getLoglevel()          const { return loglevel; }
  std::string getMap()            const { return map; }
  std::string getMapPath()        const { return mapPath; }
  int getScreen()                 const { return screen; }

  static std::string actionToString(const int &action);

private:
  void loadVideoSettings(const Json &json);
  void loadSoundSettings(const Json &json);
  void loadMouseSettings(const Json &json);
  void loadKeyboardSettings(const Json &json);
  void loadControllerSettings(const Json &json);
  void loadLoglevelSettings(const Json &json);
  void loadDefaultBindings();

  unsigned int width;
  unsigned int height;
  float mouseSensitivity;
  int antialiasing;
  int recursivePortal;
  bool loaded, fullscreen, sound, vsync, flyingEnabled
    , hidePortalsByClick, cursorVisibility
    , ignoreGlVersion, glContextEnableDebug
    , consoleEnabled, profilerEnabled, debugViewEnabled;

  Bindings bindings;

  static const Bindings defaultBindings;

  LogLevel loglevel;
  std::string map;
  std::string mapPath;
  int screen;
};

} /* namespace radix */

#endif /* RADIX_CONFIG_HPP */
