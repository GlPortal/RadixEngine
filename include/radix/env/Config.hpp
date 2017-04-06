#ifndef RADIX_CONFIG_HPP
#define RADIX_CONFIG_HPP

#include <string>

#include <json11/json11.hpp>

#include <radix/ThreadSafety.hpp>
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
  RADIX_DECLARE_SAFE_GETTER(bool, loaded);
  RADIX_DECLARE_SAFE_GETTER(unsigned int, width);
  RADIX_DECLARE_SAFE_GETTER(unsigned int, height);
  RADIX_DECLARE_SAFE_GETTER(float, sensitivity);
  RADIX_DECLARE_SAFE_GETTER(int, antialiasing);
  RADIX_DECLARE_SAFE_GETTER(int, recursivePortal);
  RADIX_DECLARE_SAFE_GETTER(bool, fullscreen);
  RADIX_DECLARE_SAFE_GETTER(bool, sound);
  RADIX_DECLARE_SAFE_GETTER(bool, vsync);
  RADIX_DECLARE_SAFE_GETTER(bool, hidePortalsByClick);
  RADIX_DECLARE_SAFE_GETTER(bool, cursorVisibility);
  RADIX_DECLARE_SAFE_GETTER(bool, ignoreGlVersion);
  RADIX_DECLARE_SAFE_GETTER(bool, glContextEnableDebug);
  RADIX_DECLARE_SAFE_GETTER(bool, consoleEnabled);
  RADIX_DECLARE_SAFE_GETTER(LogLevel, logLevel);
  RADIX_DECLARE_SAFE_GETTER(std::string, map);
  RADIX_DECLARE_SAFE_GETTER(std::string, mapPath);

  /* Config must be movable and copyable, so we have to tell the compiler how to do this,
   * since we are using mutexes in this class.
   */

  Config(Config&& other) {
    std::lock_guard<std::mutex> loadedLock(other.getloadedMutex());
    std::lock_guard<std::mutex> widthLock(other.getwidthMutex());
    std::lock_guard<std::mutex> heightLock(other.getheightMutex());
    std::lock_guard<std::mutex> sensitivityLock(other.getsensitivityMutex());
    std::lock_guard<std::mutex> antialiasingLock(other.getantialiasingMutex());
    std::lock_guard<std::mutex> recursivePortalLock(other.getrecursivePortalMutex());
    std::lock_guard<std::mutex> fullscreenLock(other.getfullscreenMutex());
    std::lock_guard<std::mutex> soundLock(other.getsoundMutex());
    std::lock_guard<std::mutex> vsyncLock(other.getvsyncMutex());
    std::lock_guard<std::mutex> hidePortalsByClickLock(other.gethidePortalsByClickMutex());
    std::lock_guard<std::mutex> cursorVisibilityLock(other.getcursorVisibilityMutex());
    std::lock_guard<std::mutex> ignoreGlVersionLock(other.getignoreGlVersionMutex());
    std::lock_guard<std::mutex> glContextEnableDebugLock(other.getglContextEnableDebugMutex());
    std::lock_guard<std::mutex> logLevelLock(other.getlogLevelMutex());
    std::lock_guard<std::mutex> mapLock(other.getmapMutex());
    std::lock_guard<std::mutex> mapPathLock(other.getmapPathMutex());

    loaded = std::move(other.getloaded());
    width = std::move(other.getwidth());
    height = std::move(other.getheight());
    sensitivity = std::move(other.getsensitivity());
    antialiasing = std::move(other.getantialiasing());
    recursivePortal = std::move(other.getrecursivePortal());
    fullscreen = std::move(other.getfullscreen());
    sound = std::move(other.getsound());
    vsync = std::move(other.getvsync());
    hidePortalsByClick = std::move(other.gethidePortalsByClick());
    cursorVisibility = std::move(other.getcursorVisibility());
    ignoreGlVersion = std::move(other.getignoreGlVersion());
    glContextEnableDebug = std::move(other.getglContextEnableDebug());
    logLevel = std::move(other.getlogLevel());
    map = std::move(other.getmap());
    mapPath = std::move(other.getmapPath());
  }

  Config(const Config &other) {
    std::lock_guard<std::mutex> loadedLock(other.getloadedMutex());
    std::lock_guard<std::mutex> widthLock(other.getwidthMutex());
    std::lock_guard<std::mutex> heightLock(other.getheightMutex());
    std::lock_guard<std::mutex> sensitivityLock(other.getsensitivityMutex());
    std::lock_guard<std::mutex> antialiasingLock(other.getantialiasingMutex());
    std::lock_guard<std::mutex> recursivePortalLock(other.getrecursivePortalMutex());
    std::lock_guard<std::mutex> fullscreenLock(other.getfullscreenMutex());
    std::lock_guard<std::mutex> soundLock(other.getsoundMutex());
    std::lock_guard<std::mutex> vsyncLock(other.getvsyncMutex());
    std::lock_guard<std::mutex> hidePortalsByClickLock(other.gethidePortalsByClickMutex());
    std::lock_guard<std::mutex> cursorVisibilityLock(other.getcursorVisibilityMutex());
    std::lock_guard<std::mutex> ignoreGlVersionLock(other.getignoreGlVersionMutex());
    std::lock_guard<std::mutex> glContextEnableDebugLock(other.getglContextEnableDebugMutex());
    std::lock_guard<std::mutex> logLevelLock(other.getlogLevelMutex());
    std::lock_guard<std::mutex> mapLock(other.getmapMutex());
    std::lock_guard<std::mutex> mapPathLock(other.getmapPathMutex());

    loaded = other.getloaded();
    width = other.getwidth();
    height = other.getheight();
    sensitivity = other.getsensitivity();
    antialiasing = other.getantialiasing();
    recursivePortal = other.getrecursivePortal();
    fullscreen = other.getfullscreen();
    sound = other.getsound();
    vsync = other.getvsync();
    hidePortalsByClick = other.gethidePortalsByClick();
    cursorVisibility = other.getcursorVisibility();
    ignoreGlVersion = other.getignoreGlVersion();
    glContextEnableDebug = other.getglContextEnableDebug();
    logLevel = other.getlogLevel();
    map = other.getmap();
    mapPath = other.getmapPath();
  }

  Config& operator = (Config &&other) {
    std::lock(getloadedMutex(), other.getloadedMutex());
    std::lock(getwidthMutex(), other.getwidthMutex());
    std::lock(getheightMutex(), other.getheightMutex());
    std::lock(getsensitivityMutex(), other.getsensitivityMutex());
    std::lock(getantialiasingMutex(), other.getantialiasingMutex());
    std::lock(getrecursivePortalMutex(), other.getrecursivePortalMutex());
    std::lock(getfullscreenMutex(), other.getfullscreenMutex());
    std::lock(getsoundMutex(), other.getsoundMutex());
    std::lock(getvsyncMutex(), other.getvsyncMutex());
    std::lock(gethidePortalsByClickMutex(), other.gethidePortalsByClickMutex());
    std::lock(getcursorVisibilityMutex(), other.getcursorVisibilityMutex());
    std::lock(getignoreGlVersionMutex(), other.getignoreGlVersionMutex());
    std::lock(getglContextEnableDebugMutex(), other.getglContextEnableDebugMutex());
    std::lock(getlogLevelMutex(), other.getlogLevelMutex());
    std::lock(getmapMutex(), other.getmapMutex());
    std::lock(getmapPathMutex(), other.getmapPathMutex());

    std::lock_guard<std::mutex> selfLoadedLock(getloadedMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfWidthLock(getwidthMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfHeightLock(getheightMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfSensitivityLock(getsensitivityMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfAntialiasingLock(getantialiasingMutex(),
                                                     std::adopt_lock);
    std::lock_guard<std::mutex> selfRecursivePortalLock(getrecursivePortalMutex(),
                                                        std::adopt_lock);
    std::lock_guard<std::mutex> selfFullscreenLock(getfullscreenMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfSoundLock(getsoundMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfVsyncLock(getvsyncMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfHidePortalsByClickLock(gethidePortalsByClickMutex(),
                                                           std::adopt_lock);
    std::lock_guard<std::mutex> selfCursorVisibilityLock(getcursorVisibilityMutex(),
                                                         std::adopt_lock);
    std::lock_guard<std::mutex> selfIgnoreGlVersionLock(getignoreGlVersionMutex(),
                                                        std::adopt_lock);
    std::lock_guard<std::mutex> selfGlContextEnableDebugLock(
      getglContextEnableDebugMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfLogLevelLock(getlogLevelMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfMapLock(getmapMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfMapPathLock(getmapPathMutex(), std::adopt_lock);

    std::lock_guard<std::mutex> otherLoadedLock(other.getloadedMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherWidthLock(other.getwidthMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherHeightLock(other.getheightMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherSensitivityLock(other.getsensitivityMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherAntialiasingLock(other.getantialiasingMutex(),
                                                      std::adopt_lock);
    std::lock_guard<std::mutex> otherRecursivePortalLock(other.getrecursivePortalMutex(),
                                                         std::adopt_lock);
    std::lock_guard<std::mutex> otherFullscreenLock(other.getfullscreenMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherSoundLock(other.getsoundMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherVsyncLock(other.getvsyncMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherHidePortalsByClickLock(other.gethidePortalsByClickMutex(),
                                                            std::adopt_lock);
    std::lock_guard<std::mutex> otherCursorVisibilityLock(other.getcursorVisibilityMutex(),
                                                          std::adopt_lock);
    std::lock_guard<std::mutex> otherIgnoreGlVersionLock(other.getignoreGlVersionMutex(),
                                                         std::adopt_lock);
    std::lock_guard<std::mutex> otherGlContextEnableDebugLock(other.
      getglContextEnableDebugMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherLogLevelLock(other.getlogLevelMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherMapLock(other.getmapMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherMapPathLock(other.getmapPathMutex(), std::adopt_lock);

    loaded = std::move(other.getloaded());
    width = std::move(other.getwidth());
    height = std::move(other.getheight());
    sensitivity = std::move(other.getsensitivity());
    antialiasing = std::move(other.getantialiasing());
    recursivePortal = std::move(other.getrecursivePortal());
    fullscreen = std::move(other.getfullscreen());
    sound = std::move(other.getsound());
    vsync = std::move(other.getvsync());
    hidePortalsByClick = std::move(other.gethidePortalsByClick());
    cursorVisibility = std::move(other.getcursorVisibility());
    ignoreGlVersion = std::move(other.getignoreGlVersion());
    glContextEnableDebug = std::move(other.getglContextEnableDebug());
    logLevel = std::move(other.getlogLevel());
    map = std::move(other.getmap());
    mapPath = std::move(other.getmapPath());

    return *this;
  }

  Config& operator = (const Config &other) {
    std::lock(getloadedMutex(), other.getloadedMutex());
    std::lock(getwidthMutex(), other.getwidthMutex());
    std::lock(getheightMutex(), other.getheightMutex());
    std::lock(getsensitivityMutex(), other.getsensitivityMutex());
    std::lock(getantialiasingMutex(), other.getantialiasingMutex());
    std::lock(getrecursivePortalMutex(), other.getrecursivePortalMutex());
    std::lock(getfullscreenMutex(), other.getfullscreenMutex());
    std::lock(getsoundMutex(), other.getsoundMutex());
    std::lock(getvsyncMutex(), other.getvsyncMutex());
    std::lock(gethidePortalsByClickMutex(), other.gethidePortalsByClickMutex());
    std::lock(getcursorVisibilityMutex(), other.getcursorVisibilityMutex());
    std::lock(getignoreGlVersionMutex(), other.getignoreGlVersionMutex());
    std::lock(getglContextEnableDebugMutex(), other.getglContextEnableDebugMutex());
    std::lock(getlogLevelMutex(), other.getlogLevelMutex());
    std::lock(getmapMutex(), other.getmapMutex());
    std::lock(getmapPathMutex(), other.getmapPathMutex());

    std::lock_guard<std::mutex> selfLoadedLock(getloadedMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfWidthLock(getwidthMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfHeightLock(getheightMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfSensitivityLock(getsensitivityMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfAntialiasingLock(getantialiasingMutex(),
                                                     std::adopt_lock);
    std::lock_guard<std::mutex> selfRecursivePortalLock(getrecursivePortalMutex(),
                                                        std::adopt_lock);
    std::lock_guard<std::mutex> selfFullscreenLock(getfullscreenMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfSoundLock(getsoundMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfVsyncLock(getvsyncMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfHidePortalsByClickLock(gethidePortalsByClickMutex(),
                                                           std::adopt_lock);
    std::lock_guard<std::mutex> selfCursorVisibilityLock(getcursorVisibilityMutex(),
                                                         std::adopt_lock);
    std::lock_guard<std::mutex> selfIgnoreGlVersionLock(getignoreGlVersionMutex(),
                                                        std::adopt_lock);
    std::lock_guard<std::mutex> selfGlContextEnableDebugLock(
      getglContextEnableDebugMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfLogLevelLock(getlogLevelMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfMapLock(getmapMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> selfMapPathLock(getmapPathMutex(), std::adopt_lock);

    std::lock_guard<std::mutex> otherLoadedLock(other.getloadedMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherWidthLock(other.getwidthMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherHeightLock(other.getheightMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherSensitivityLock(other.getsensitivityMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherAntialiasingLock(other.getantialiasingMutex(),
                                                      std::adopt_lock);
    std::lock_guard<std::mutex> otherRecursivePortalLock(other.getrecursivePortalMutex(),
                                                         std::adopt_lock);
    std::lock_guard<std::mutex> otherFullscreenLock(other.getfullscreenMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherSoundLock(other.getsoundMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherVsyncLock(other.getvsyncMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherHidePortalsByClickLock(other.gethidePortalsByClickMutex(),
                                                            std::adopt_lock);
    std::lock_guard<std::mutex> otherCursorVisibilityLock(other.getcursorVisibilityMutex(),
                                                          std::adopt_lock);
    std::lock_guard<std::mutex> otherIgnoreGlVersionLock(other.getignoreGlVersionMutex(),
                                                         std::adopt_lock);
    std::lock_guard<std::mutex> otherGlContextEnableDebugLock(other.
      getglContextEnableDebugMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherLogLevelLock(other.getlogLevelMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherMapLock(other.getmapMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> otherMapPathLock(other.getmapPathMutex(), std::adopt_lock);

    loaded = other.getloaded();
    width = other.getwidth();
    height = other.getheight();
    sensitivity = other.getsensitivity();
    antialiasing = other.getantialiasing();
    recursivePortal = other.getrecursivePortal();
    fullscreen = other.getfullscreen();
    sound = other.getsound();
    vsync = other.getvsync();
    hidePortalsByClick = other.gethidePortalsByClick();
    cursorVisibility = other.getcursorVisibility();
    ignoreGlVersion = other.getignoreGlVersion();
    glContextEnableDebug = other.getglContextEnableDebug();
    logLevel = other.getlogLevel();
    map = other.getmap();
    mapPath = other.getmapPath();

    return *this;
  }

private:
  void loadVideoSettings(Json json);
  void loadSoundSettings(Json json);
  void loadMouseSettings(Json json);
  void loadLoglevelSettings(Json json);

  RADIX_DECLARE_WITH_MUTEX(bool, loaded);
  RADIX_DECLARE_WITH_MUTEX(unsigned int, width);
  RADIX_DECLARE_WITH_MUTEX(unsigned int, height);
  RADIX_DECLARE_WITH_MUTEX(float, sensitivity);
  RADIX_DECLARE_WITH_MUTEX(int, antialiasing);
  RADIX_DECLARE_WITH_MUTEX(int, recursivePortal);
  RADIX_DECLARE_WITH_MUTEX(bool, fullscreen);
  RADIX_DECLARE_WITH_MUTEX(bool, sound);
  RADIX_DECLARE_WITH_MUTEX(bool, vsync);
  RADIX_DECLARE_WITH_MUTEX(bool, hidePortalsByClick);
  RADIX_DECLARE_WITH_MUTEX(bool, cursorVisibility);
  RADIX_DECLARE_WITH_MUTEX(bool, ignoreGlVersion);
  RADIX_DECLARE_WITH_MUTEX(bool, glContextEnableDebug);
  RADIX_DECLARE_WITH_MUTEX(bool, consoleEnabled);
  RADIX_DECLARE_WITH_MUTEX(LogLevel, logLevel);
  RADIX_DECLARE_WITH_MUTEX(std::string, map);
  RADIX_DECLARE_WITH_MUTEX(std::string, mapPath);
};

} /* namespace radix */

#endif /* RADIX_CONFIG_HPP */
