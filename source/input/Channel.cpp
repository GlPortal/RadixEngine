#include <radix/input/Channel.hpp>

#include <radix/core/diag/Throwables.hpp>
#include <radix/input/InputSource.hpp>
#include <radix/input/InputManager.hpp>

#include <cstdlib>
#include <ciso646>

namespace radix {
  
void DigitalChannel::init(const int &id, EventDispatcher &event, const std::vector<Bind> &binds) {
  if (this->listeners.empty()) {
    throw Exception::Error("Channel", "Tried to initialise channel, id: " + std::to_string(id) + ", without a listener");
  }

  this->setId(id);

  if (InputManager::isActionDigital(id)) {
    this->setDigital(0.5f);

    if (id == InputManager::PLAYER_JUMP) {
      this->setAutoZero();
    }
  } else {
    this->setAnalogue(0.0f);
  }

  for (auto &bind: binds) {
    this->subChannels.push_back(SubChannel<float>(this));
    this->subChannels.back().init(id, event, bind);
  }
}
  
void DigitalChannel::reInit(EventDispatcher &event) {
  this->notifyListeners();

  for (auto &subChannel : subChannels) {
    subChannel.reInit(event);
  }
}

void DigitalChannel::channelChanged(float newValue, const int &id) {
  this->set(newValue > 0.5f ? true : false);
}
  
void VectorChannel::init(const int &id, EventDispatcher &event, const std::vector<Bind> &binds) {
  if (this->listeners.empty()) {
    throw Exception::Error("Channel", "Tried to initialise channel, id: " + std::to_string(id) + ", without a listener");
  }

  setId(id);

  this->setAnalogue(0.0f);

  for (auto &bind: binds) {
    this->subChannels.push_back(SubChannel<Vector2f>(this));
    this->subChannels.back().init(bind.action, event, bind);
  }
}
  
void VectorChannel::reInit(EventDispatcher &event) {
  this->notifyListeners();

  for (auto &subChannel : subChannels) {
    subChannel.reInit(event);
  }
}

void VectorChannel::channelChanged(Vector2f newValue, const int &id) {
  this->set(newValue);
}

template<class T>
void SubChannel<T>::init(const int &id, EventDispatcher &event, const Bind& bind) {
  if (this->listeners.empty()) {
    throw Exception::Error("SubChannel::init()", "Tried to initialise sub-channel, id: " + std::to_string(id) + ", without a listener");
  }

  this->setId(id);
  this->bind = bind;
  this->setSensitivity(bind.sensitivity);

  if (InputManager::isActionDigital(bind.action) or Bind::isInputTypeDigital(bind.inputType)) {
    this->setDigital(bind.actPoint);
  } else {
    if (bind.inputType == Bind::CONTROLLER_AXIS) {
      this->setAnalogue(bind.deadZone);
      if (bind.action != InputManager::PLAYER_LOOK_ANALOGUE){
        this->setBound(0.9f);
      }
    } else if (bind.inputType == Bind::MOUSE_AXIS) {
      this->setAnalogue(0.0f);
      this->setAutoZero();
    }
  }

  this->addObservers(event);
}

template<class T>
void SubChannel<T>::reInit(EventDispatcher &event) {
  this->addObservers(event);
}

template<class T>
void SubChannel<T>::addObservers(EventDispatcher &event) {
  switch (bind.inputType) {
  case Bind::KEYBOARD: {
    event.addObserverRaw(InputSource::KeyPressedEvent::Type, [this](const radix::Event& event) {
        const int key = ((radix::InputSource::KeyPressedEvent &) event).key;
        if (key == this->bind.inputCode) {
          this->set(1.0f);
        }
      });

    event.addObserverRaw(InputSource::KeyReleasedEvent::Type, [this](const radix::Event& event) {
        const int key = ((radix::InputSource::KeyReleasedEvent &) event).key;
        if (key == this->bind.inputCode) {
          this->set(0.0f);
        }
      });
    break;
  }
  case Bind::MOUSE_BUTTON: {
    event.addObserverRaw(InputSource::MouseButtonPressedEvent::Type, [this](const radix::Event& event) {
        const int button = (int)((radix::InputSource::MouseButtonPressedEvent &) event).button;
        if (button == this->bind.inputCode) {
          this->set(1.0f);
        }
      });

    event.addObserverRaw(InputSource::MouseButtonReleasedEvent::Type, [this](const radix::Event& event) {
        const int button = (int)((radix::InputSource::MouseButtonPressedEvent &) event).button;
        if (button == this->bind.inputCode) {
          this->set(0.0f);
        }
      });
    break;
  }
  case Bind::CONTROLLER_BUTTON: {
    event.addObserverRaw(InputSource::ControllerButtonPressedEvent::Type, [this](const radix::Event& event) {
        const int button = ((radix::InputSource::ControllerButtonPressedEvent &) event).button;
        if (button == this->bind.inputCode) {
          this->set(1.0f);
        }
      });

    event.addObserverRaw(InputSource::ControllerButtonReleasedEvent::Type, [this](const radix::Event& event) {
        const int button = ((radix::InputSource::ControllerButtonReleasedEvent &) event).button;
        if (button == this->bind.inputCode) {
          this->set(0.0f);
        }
      });
    break;
  }
  case Bind::CONTROLLER_TRIGGER: {
    event.addObserverRaw(InputSource::ControllerTriggerEvent::Type, [this](const radix::Event& event) {
        const int trigger = ((radix::InputSource::ControllerTriggerEvent &) event).trigger;
        const float value = ((radix::InputSource::ControllerTriggerEvent &) event).value;
        if (trigger == this->bind.inputCode) {
          this->set(value);
        }
      });
    break;
  }
  }
}

template<>
void SubChannel<Vector2f>::addObservers(EventDispatcher &event) {
  switch (bind.inputType) {
  case Bind::MOUSE_AXIS: {
    event.addObserverRaw(InputSource::MouseAxisEvent::Type, [this](const radix::Event& event) {
        const Vector2f value = ((radix::InputSource::MouseAxisEvent &) event).value;
    
        this->set(value);
      });
    break;
  }
  case Bind::CONTROLLER_AXIS: {
    event.addObserverRaw(InputSource::ControllerAxisEvent::Type, [this](const radix::Event& event) {
        const int axis = ((radix::InputSource::ControllerAxisEvent &) event).axis;
        const Vector2f value = ((radix::InputSource::ControllerAxisEvent &) event).value;

        if (axis == this->bind.inputCode) {
          this->set(value);
        }
      });
    break;
  }
  }
}

template class SubChannel<float>;
template class SubChannel<Vector2f>;

}
