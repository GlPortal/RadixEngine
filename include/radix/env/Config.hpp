#ifndef RADIX_CONFIG_HPP
#define RADIX_CONFIG_HPP

#include <string>

#include <json11/json11.hpp>

#include <radix/core/diag/Logger.hpp>
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
  Config();
  void load();
  bool isLoaded();
  unsigned int getWidth() const { return width; }
  unsigned int getHeight() const { return height; }
  float getSensitivity() const { return sensitivity; }
  bool isFullscreen() const { return fullscreen; }
  int getAntialiasLevel() const { return antialiasing; }
  int getRecursionLevel() const { return recursivePortal; }
  bool hasSound() const { return sound; }
  bool hasVsync() const { return vsync; }
  bool isHidePortalsByClick() const { return hidePortalsByClick; }
  bool isConsoleEnabled() const { return consoleEnabled; }
  bool isProfilerEnabled() const { return profilerEnabled; }
  bool getCursorVisibility() const { return cursorVisibility; }
  bool getIgnoreGlVersion() const { return ignoreGlVersion; }
  bool getGlContextEnableDebug() const { return glContextEnableDebug; }
  LogLevel getLoglevel() const { return loglevel; }
  std::string getMap() const { return map; }
  std::string getMapPath() const { return mapPath; }

private:
  void loadVideoSettings(const Json &json);
  void loadSoundSettings(const Json &json);
  void loadMouseSettings(const Json &json);
  void loadLoglevelSettings(const Json &json);

  bool loaded;
  unsigned int width;
  unsigned int height;
  float sensitivity;
  int antialiasing;
  int recursivePortal;
  bool fullscreen;
  bool sound;
  bool vsync;
  bool hidePortalsByClick;
  bool cursorVisibility;
  bool ignoreGlVersion;
  bool glContextEnableDebug;
  bool consoleEnabled;
  bool profilerEnabled;
  LogLevel loglevel;
  std::string map;
  std::string mapPath;
};

} /* namespace radix */

#endif /* RADIX_CONFIG_HPP */
