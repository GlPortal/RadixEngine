#include <radix/env/Config.hpp>

#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <array>

#include <radix/env/Environment.hpp>
#include <radix/env/Util.hpp>
#include <radix/input/InputManager.hpp>
#include <radix/input/InputSource.hpp>
#include <radix/input/Bind.hpp>

namespace radix {

std::string Config::actionToString(const int &action) {
  switch (InputManager::Action(action)) {
    case InputManager::PLAYER_JUMP :      return "jump";
    case InputManager::PLAYER_PORTAL_0 :  return "portal_0";
    case InputManager::PLAYER_PORTAL_1 :  return "portal_1";
    case InputManager::PLAYER_MOVE_X :    return "move_x";
    case InputManager::PLAYER_MOVE_Y :    return "move_y";
    case InputManager::PLAYER_LOOK_X :    return "look_x";
    case InputManager::PLAYER_LOOK_Y :    return "look_y";
    case InputManager::PLAYER_FORWARD :   return "forward";
    case InputManager::PLAYER_BACK :      return "back";
    case InputManager::PLAYER_LEFT :      return "left";
    case InputManager::PLAYER_RIGHT :     return "right";
    case InputManager::GAME_PAUSE :       return "pause";
    case InputManager::GAME_QUIT :        return "quit";
    case InputManager::ACTION_INVALID :
    case InputManager::ACTION_MAX :
    default :                             return "invalid";
  }
}

Config::Config() :
  loaded(false),
  ignoreGlVersion(false),
  consoleEnabled(false),
  profilerEnabled(false),
  debugViewEnabled(false) {
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
  this->loadControllerSettings(configJson["controller"]);
  this->loadKeyboardSettings(configJson["keyboard"]);
  this->loadLoglevelSettings(configJson["logging"]);

  const Json &debug = configJson["debug"];
  glContextEnableDebug = debug["gl_context_debug"].bool_value();
  profilerEnabled = debug["profiler"]["enable"].bool_value();
  debugViewEnabled = debug["wireframes"]["enable"].bool_value();
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
  sound = json["enable"].bool_value();
}

void Config::loadMouseSettings(const Json &json) {
  mouseSensitivity   = json["sensitivity"].number_value() / 500.0f;
  hidePortalsByClick = json["hide_portals_by_click"].bool_value();
  cursorVisibility   = json["cursor_visibility"].bool_value();

  for (int action = 0; action < int(InputManager::Action::ACTION_MAX); ++action) {
    std::string actionStr = actionToString(action);
    //if (!json[actionstr].is_array()) {
    //  break;
    //}
    
    for (int index = 0; index < 5; ++index) {
      std::string buttonStr = json["bindings"][actionStr][index].string_value();
      if (buttonStr == "") {
        break;
      } else {
        int button = InputSource::mouseGetButtonFromString(buttonStr);
        int axis = InputSource::mouseGetAxisFromString(buttonStr);

        if (button != -1) {
          Bind bind(button, Bind::MOUSE_BUTTON, action);
          bindings.push_back(bind);
          Util::Log(Warning, "Config") << buttonStr << " bound to " << actionStr;
        } else if (axis != -1) {
          Bind bind(axis, Bind::MOUSE_AXIS, action, mouseSensitivity);
          bindings.push_back(bind);
          Util::Log(Warning, "Config") << buttonStr << " bound to " << actionStr << " with sensitivity " << mouseSensitivity;
        } else {
          Util::Log(Warning, "Config") << buttonStr << "is an invalid control name";
        }
      }
    }
  }
}

void Config::loadKeyboardSettings(const Json &json) {
  for (int action = 0; action < InputManager::ACTION_MAX; ++action) {
    std::string actionStr = actionToString(action);
    //if (!json[actionstr].is_array()) {
    //  break;
    //}

    for (int index = 0; index < 5; ++index) {
      std::string keyStr = json["bindings"][actionStr][index].string_value();
      if (keyStr == "") {
        break;
      } else {
        int key = InputSource::keyboardGetKeyFromString(keyStr);

        if (key != 0) {
          Bind bind(key, Bind::KEYBOARD, action);
          bindings.push_back(bind);
          Util::Log(Warning, "Config") << keyStr << " bound to " << actionStr;
        } else {
          Util::Log(Warning, "Config") << keyStr << "is an invalid control name";
        }
      }
    }
  }
}

void Config::loadControllerSettings(const Json &json) {
  for (int action = 0; action < int(InputManager::Action::ACTION_MAX); ++action) {
    std::string actionStr = actionToString(action);
    //if (!json[actionstr].is_array()) {
    //  break;
    //}
    
    for (int index = 0; index < 5; ++index) {
      std::string buttonStr = json["bindings"][actionStr][index].string_value();
      if (buttonStr == "") {
        break;
      } else {
        int button = InputSource::gameControllerGetButtonFromString(buttonStr);
        int axis = InputSource::gameControllerGetAxisFromString(buttonStr);

        if (button != -1) {
          Bind bind(button, Bind::CONTROLLER_BUTTON, action);
          bindings.push_back(bind);
          Util::Log(Warning, "Config") << buttonStr << " bound to " << actionStr;
        } else if (axis != -1) {
          float sensitivity = json["sensitivity"][buttonStr].number_value();
          int deadZone = json["dead_zone"][buttonStr].number_value();
          Bind bind(axis, Bind::CONTROLLER_AXIS, action, sensitivity, deadZone);
          bindings.push_back(bind);
          Util::Log(Warning, "Config") << buttonStr << " bound to " << actionStr << " with sensitivity " << sensitivity << " and deadzone " << deadZone;
        } else {
          Util::Log(Warning, "Config") << buttonStr << "is an invalid control name";
        }
      }
    }
  }
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

