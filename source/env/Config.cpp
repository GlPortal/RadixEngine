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

namespace radix {

const std::vector<std::vector<Bind>> Config::defaultBindings = {
  {Bind()},
  {Bind(InputManager::PLAYER_LOOK_ANALOGUE, Bind::MOUSE_AXIS, 0, 1.0f)},
  {Bind(InputManager::PLAYER_JUMP, Bind::KEYBOARD, InputSource::keyboardGetKeyFromString("Space"))},
  {Bind(InputManager::PLAYER_PORTAL_0, Bind::MOUSE_BUTTON, (int)InputSource::MouseButton::Left)},
  {Bind(InputManager::PLAYER_PORTAL_1, Bind::MOUSE_BUTTON, (int)InputSource::MouseButton::Right)},
  {Bind(InputManager::PLAYER_FORWARD, Bind::KEYBOARD, InputSource::keyboardGetKeyFromString("W"))},
  {Bind(InputManager::PLAYER_BACK, Bind::KEYBOARD, InputSource::keyboardGetKeyFromString("S"))},
  {Bind(InputManager::PLAYER_LEFT, Bind::KEYBOARD, InputSource::keyboardGetKeyFromString("A"))},
  {Bind(InputManager::PLAYER_RIGHT, Bind::KEYBOARD, InputSource::keyboardGetKeyFromString("D"))},
  {Bind(InputManager::GAME_PAUSE, Bind::KEYBOARD, InputSource::keyboardGetKeyFromString("Escape"))},
  {Bind(InputManager::GAME_QUIT, Bind::KEYBOARD, InputSource::keyboardGetKeyFromString("Q"))}
};

std::string Config::actionToString(const int &action) {
  switch (InputManager::Action(action)) {
    case InputManager::PLAYER_JUMP :          return "jump";
    case InputManager::PLAYER_PORTAL_0 :      return "portal_0";
    case InputManager::PLAYER_PORTAL_1 :      return "portal_1";
    case InputManager::PLAYER_MOVE_ANALOGUE : return "move_analogue";
    case InputManager::PLAYER_LOOK_ANALOGUE : return "look_analogue";
    case InputManager::PLAYER_FORWARD :       return "forward";
    case InputManager::PLAYER_BACK :          return "back";
    case InputManager::PLAYER_LEFT :          return "left";
    case InputManager::PLAYER_RIGHT :         return "right";
    case InputManager::GAME_PAUSE :           return "pause";
    case InputManager::GAME_QUIT :            return "quit";
    case InputManager::ACTION_INVALID :
    case InputManager::ACTION_MAX :
    default :                                 return "";
  }
}

Config::Config() :
  loaded(false),
  fullscreen(false),
  sound(false),
  vsync(false),
  flyingEnabled(false),
  hidePortalsByClick(false),
  cursorVisibility(false),
  ignoreGlVersion(false),
  glContextEnableDebug(false),
  consoleEnabled(false),
  profilerEnabled(false),
  debugViewEnabled(false),
  bindings(InputManager::ACTION_MAX, std::vector<Bind>()),
  screen(0) {}

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
  this->loadDefaultBindings();
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
  screen          = json["screen"].number_value();
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
    
    for (int index = 0; index < 5; ++index) {
      std::string buttonStr = json["bindings"][actionStr][index].string_value();
      if (buttonStr == "") {
        break;
      } else {
        int button = InputSource::mouseGetButtonFromString(buttonStr);
        bool axis = buttonStr == "mouse_move";

        if (button != -1) {
          Bind bind(action, Bind::MOUSE_BUTTON, button);
          bindings[action].push_back(bind);
          Util::Log(Info, "Config") << buttonStr << button << " bound to " << actionStr;
        } else if (axis) {
          Bind bind(action, Bind::MOUSE_AXIS, 0, mouseSensitivity, 0);
          bindings[action].push_back(bind);
          Util::Log(Info, "Config") << buttonStr << " bound to " << actionStr << " with sensitivity " << mouseSensitivity;
        } else {
          Util::Log(Info, "Config") << buttonStr << "is an invalid control name";
        }
      }
    }
  }
}

void Config::loadKeyboardSettings(const Json &json) {
  for (int action = 0; action < InputManager::ACTION_MAX; ++action) {
    std::string actionStr = actionToString(action);

    for (int index = 0; index < 5; ++index) {
      std::string keyStr = json["bindings"][actionStr][index].string_value();
      if (keyStr == "") {
        break;
      } else {
        int key = InputSource::keyboardGetKeyFromString(keyStr);

        if (key > 0) {
          Bind bind(action, Bind::KEYBOARD, key);
          bindings[action].push_back(bind);
          Util::Log(Info, "Config") << keyStr << " bound to " << actionStr;
        } else {
          Util::Log(Info, "Config") << keyStr << "is an invalid control name";
        }
      }
    }
  }
}

void Config::loadControllerSettings(const Json &json) {
  for (int action = 0; action < int(InputManager::Action::ACTION_MAX); ++action) {
    std::string actionStr = actionToString(action);

    for (int index = 0; index < 5; ++index) {
      std::string buttonStr = json["bindings"][actionStr][index].string_value();
      if (buttonStr == "") {
        break;
      } else {
        int button  = InputSource::gameControllerGetButtonFromString(buttonStr);
        int axis    = InputSource::gameControllerGetAxisFromString(buttonStr);
        int trigger = InputSource::gameControllerGetTriggerFromString(buttonStr);

        if (button != -1) {
          Bind bind(action, Bind::CONTROLLER_BUTTON, button);
          bindings[action].push_back(bind);
          Util::Log(Info, "Config") << buttonStr << " bound to " << actionStr;
        } else if (axis != -1) {
          float sensitivity = json["sensitivity"][buttonStr].number_value();
          float deadZone = json["dead_zone"][buttonStr].number_value();
          Bind bind(action, Bind::CONTROLLER_AXIS, axis, sensitivity, deadZone);
          bindings[action].push_back(bind);
          Util::Log(Info, "Config") << buttonStr << " bound to " << actionStr << " with sensitivity " << sensitivity << " and deadzone " << deadZone;
        } else if (trigger != -1) {
          float sensitivity = json["sensitivity"][buttonStr].number_value();
          float actPoint = json["dead_zone"][buttonStr].number_value();
          Bind bind(action, Bind::CONTROLLER_TRIGGER, trigger, sensitivity, actPoint);
          bindings[action].push_back(bind);
        } else {
          Util::Log(Info, "Config") << buttonStr << "is an invalid control name";
        }
      }
    }
  }
}

void Config::loadDefaultBindings() {
  for (int i(0); i < InputManager::ACTION_MAX; ++i) {
    if (bindings[i].empty()) {
      bindings[i] = defaultBindings[i];
      Util::Log(Info, "Config") << actionToString(i) << " set to default bind";
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

