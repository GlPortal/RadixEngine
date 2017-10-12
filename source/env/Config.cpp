#include <radix/env/Config.hpp>

#include <cstring>
#include <fstream>

#include <radix/env/Environment.hpp>

namespace radix {

Config::Config() :
  loaded(false),
  ignoreGlVersion(false),
  consoleEnabled(false),
  flyingEnabled(false),
  profilerEnabled(false) {
}


void Config::load() {
  std::string err;
  std::string templateTxt;
  std::string templatePath = Environment::getDataDir() + "/config.json";
  std::ifstream in(templatePath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    templateTxt.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&templateTxt[0], templateTxt.size());
    in.close();
  } else {
    throw std::runtime_error(std::string("Can't open ") +
                             templatePath + ": " +
                             std::strerror(errno));
  }
  Json configJson = Json::parse(templateTxt, err);

  this->loadVideoSettings(configJson["video"]);
  this->loadSoundSettings(configJson["sound"]);
  this->loadMouseSettings(configJson["mouse"]);
  this->loadLoglevelSettings(configJson["logging"]);

  const Json &debug = configJson["debug"];
  glContextEnableDebug = debug["gl_context_debug"].bool_value();
  profilerEnabled = debug["profiler"]["enable"].bool_value();
  flyingEnabled = debug["flying"]["enable"].bool_value();
  loaded = true;
}

bool Config::isLoaded() {
  return loaded;
}

void Config::loadVideoSettings(const Json &json) {
  fullscreen      = json["fullscreen"].bool_value();
  antialiasing    = json["antialiasing"].number_value();
  vsync           = json["vsync"].bool_value();
  width           = json["width"].number_value();
  height          = json["height"].number_value();
  recursivePortal = json["recursive_portal"].number_value();

}

void Config::loadSoundSettings(const Json &json) {
  sound = json["enabled"].bool_value();

}

void Config::loadMouseSettings(const Json &json) {
  sensitivity        = json["sensitivity"].number_value();
  hidePortalsByClick = json["hide_portals_by_click"].bool_value();
  cursorVisibility   = json["cursor_visibility"].bool_value();
}

void Config::loadLoglevelSettings(const Json &json) {
  std::string value = json["loglevel"].string_value();
  if (value == "verbose") {
    loglevel = LogLevel::Verbose;
  } else if (value == "debug") {
    loglevel = LogLevel::Debug;
  } else if (value == "info") {
    loglevel = LogLevel::Info;
  } else if (value == "warning") {
    loglevel = LogLevel::Warning;
  } else if (value == "error") {
    loglevel = LogLevel::Error;
  } else if (value == "failure") {
    loglevel = LogLevel::Failure;
  } else {
    loglevel = LogLevel::Debug;
  }
}

} /* namespace radix */
