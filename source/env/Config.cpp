#include <radix/env/Config.hpp>

#include <string>
#include <cstring>
#include <fstream>
#include <stdexcept>

#include <json11/json11.hpp>

#include <radix/env/Environment.hpp>

using namespace json11;

namespace radix {

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
  Json templateJson = Json::parse(templateTxt, err);
  Json videoJson = templateJson["video"];
  Json soundJson = templateJson["sound"];
  Json mouseJson = templateJson["mouse"];

  // Video
  fullscreen      = videoJson["fullscreen"].bool_value();
  antialiasing    = videoJson["antialiasing"].number_value();
  vsync           = videoJson["vsync"].bool_value();
  width           = videoJson["width"].number_value();
  height          = videoJson["height"].number_value();
  recursivePortal = videoJson["recursive_portal"].number_value();

  // Sound
  sound = soundJson["enabled"].bool_value();

  // Mouse
  sensitivity        = mouseJson["sensitivity"].number_value();
  hidePortalsByClick = mouseJson["hide_portals_by_click"].bool_value();
  cursorVisibility   = mouseJson["cursor_visibility"].bool_value();

  // Misc
  map = "n1";
}

} /* namespace radix */
