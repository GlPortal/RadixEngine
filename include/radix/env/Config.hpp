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
  unsigned int getWidth() { return width; }
  unsigned int getHeight() { return height; }
  float getSensitivity() { return sensitivity; }
  bool isFullscreen() { return fullscreen; }
  int getAntialiasLevel() { return antialiasing; }
  int getRecursionLevel() { return recursivePortal; }
  bool hasSound() { return sound; }
  bool hasVsync() { return vsync; }
  bool isHidePortalsByClick() { return hidePortalsByClick; }
  bool getCursorVisibility() { return cursorVisibility; }
  bool getIgnoreGlVersion() { return ignoreGlVersion; }
  LogLevel getLoglevel() { return loglevel; }
  std::string getMap() { return map; }
  std::string getMapPath() { return mapPath; }

private:
  void loadVideoSettings(Json json);
  void loadSoundSettings(Json json);
  void loadMouseSettings(Json json);
  void loadLoglevelSettings(Json json);

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
  LogLevel loglevel;
  std::string map;
  std::string mapPath;
};

} /* namespace radix */

#endif /* RADIX_CONFIG_HPP */
