#include <radix/env/Config.hpp>

#include <cstring>
#include <fstream>

#include <radix/env/Environment.hpp>

namespace radix {

Config::Config() : loaded(false), ignoreGlVersion(false), consoleEnabled(false) {
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

  glContextEnableDebug = configJson["debug"]["gl_context_debug"].bool_value();
  map = "n1";
  loaded = true;
}

void Config::loadVideoSettings(Json json) {
  fullscreen      = json["fullscreen"].bool_value();
  antialiasing    = json["antialiasing"].number_value();
  vsync           = json["vsync"].bool_value();
  width           = json["width"].number_value();
  height          = json["height"].number_value();
  recursivePortal = json["recursive_portal"].number_value();

}

void Config::loadSoundSettings(Json json) {
  sound = json["enabled"].bool_value();

}

void Config::loadMouseSettings(Json json) {
  sensitivity        = json["sensitivity"].number_value();
  hidePortalsByClick = json["hide_portals_by_click"].bool_value();
  cursorVisibility   = json["cursor_visibility"].bool_value();
}

void Config::loadLoglevelSettings(Json json) {
  std::string value = json["loglevel"].string_value();
  if (value == "verbose") {
    logLevel = LogLevel::Verbose;
  } else if (value == "debug") {
    logLevel = LogLevel::Debug;
  } else if (value == "info") {
    logLevel = LogLevel::Info;
  } else if (value == "warning") {
    logLevel = LogLevel::Warning;
  } else if (value == "error") {
    logLevel = LogLevel::Error;
  } else if (value == "failure") {
    logLevel = LogLevel::Failure;
  } else {
    logLevel = LogLevel::Debug;
  }
}

} /* namespace radix */
