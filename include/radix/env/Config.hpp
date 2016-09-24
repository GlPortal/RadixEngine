#ifndef RADIX_CONFIG_HPP
#define RADIX_CONFIG_HPP

#include <string>

#include <json11/json11.hpp>
using namespace json11;

namespace radix {

/** @brief Configuration class
 *
 *  Load and represent the configuration
 */
class Config {
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

  std::string map;
  std::string mapPath;
  bool cursorVisibility;
private:
  void loadVideoSettings(Json json);
  void loadSoundSettings(Json json);
  void loadMouseSettings(Json json);
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
};

} /* namespace radix */

#endif /* RADIX_CONFIG_HPP */
