#include <radix/input/Channel.hpp>

#include <radix/core/diag/Throwables.hpp>
#include <radix/input/InputSource.hpp>
#include <radix/input/InputManager.hpp>

#include <cstdlib>

namespace radix {

template<class T>
void Channel<T>::init(const int &id, EventDispatcher &event, const std::vector<Bind> &binds) {
	if (this->listeners.empty()) {
		throw Exception::Error("Channel", "Tried to initialise sub-channel, id: " + std::to_string(id) + ", without a listener");
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

	for (int i = 0; i < (int)binds.size(); ++i) {
		this->subChannels.push_back(SubChannel<T>(this));
	}
	//these have to be separate for some reason
	for (int i = 0; i < (int)binds.size(); ++i) {
		this->subChannels[i].init(i, event, binds[i]);
	}
}

template<class T>
void Channel<T>::channelChanged(const int &id) {
	this->Channel<T>::set(this->subChannels.at(id).SubChannel<T>::get());
}

template<class T>
void SubChannel<T>::init(const int &id, EventDispatcher &event, const Bind& bind) {
	if (this->listeners.empty()) {
		throw Exception::Error("SubChannel", "Tried to initialise sub-channel, id: " + std::to_string(id) + ", without a listener");
	}

	this->setId(id);
	this->bind = bind;
	this->setSensitivity(bind.sensitivity);

	if (InputManager::isActionDigital(bind.action) or Bind::isInputTypeDigital(bind.inputType)) {
		this->setDigital(bind.actPoint);
	} else {
		this->setAnalogue(bind.deadZone);
		this->setBound(1.0f);
	}

	switch (bind.inputType) {

	case Bind::KEYBOARD: {
		this->callbacks[0] = event.addObserver(InputSource::KeyPressedEvent::Type, [this](const radix::Event& event) {
				const int key = ((radix::InputSource::KeyPressedEvent &) event).key;
				if (key == this->bind.inputCode) {
					this->set(1.0f);
				}
			});

		this->callbacks[1] = event.addObserver(InputSource::KeyReleasedEvent::Type, [this](const radix::Event& event) {
				const int key = ((radix::InputSource::KeyReleasedEvent &) event).key;
				if (key == this->bind.inputCode) {
					this->set(0.0f);
				}
			});
		break;
	}
	case Bind::MOUSE_BUTTON: {
		this->callbacks[0] = event.addObserver(InputSource::MouseButtonPressedEvent::Type, [this](const radix::Event& event) {
				const int button = (int)((radix::InputSource::MouseButtonPressedEvent &) event).button;
				if (button == this->bind.inputCode) {
					this->set((T)1.0f);
				}
			});

		this->callbacks[1] = event.addObserver(InputSource::MouseButtonReleasedEvent::Type, [this](const radix::Event& event) {
				const int button = (int)((radix::InputSource::MouseButtonPressedEvent &) event).button;
				if (button == this->bind.inputCode) {
					this->set((T)0.0f);
				}
			});
		break;
	}
	case Bind::CONTROLLER_BUTTON: {
		this->callbacks[0] = event.addObserver(InputSource::ControllerButtonPressedEvent::Type, [this](const radix::Event& event) {
				const int button = ((radix::InputSource::ControllerButtonPressedEvent &) event).button;
				if (button == this->bind.inputCode) {
					this->set((T)1.0f);
				}
			});

		this->callbacks[1] = event.addObserver(InputSource::ControllerButtonReleasedEvent::Type, [this](const radix::Event& event) {
				const int button = ((radix::InputSource::ControllerButtonReleasedEvent &) event).button;
				if (button == this->bind.inputCode) {
					this->set((T)0.0f);
				}
			});
		break;
	}
	case Bind::CONTROLLER_TRIGGER: {
		this->callbacks[0] = event.addObserver(InputSource::ControllerTriggerEvent::Type, [this](const radix::Event& event) {
				const int trigger = ((radix::InputSource::ControllerTriggerEvent &) event).trigger;
				const float value = ((radix::InputSource::ControllerTriggerEvent &) event).value;
				if (trigger == this->bind.inputCode) {
					this->set((T)value);
				}
			});
		break;
	}
	}
}

template<>
void SubChannel<Vector2f>::init(const int &id, EventDispatcher &event, const Bind& bind) {
	if (this->listeners.empty()) {
		throw Exception::Error("SubChannel", "Tried to initialise sub-channel, id: " + std::to_string(id) + ", without a listener");
	}

	this->setId(id);
	this->bind = bind;
	this->setSensitivity(bind.sensitivity);
	if (InputManager::isActionDigital(bind.action) or Bind::isInputTypeDigital(bind.inputType)) {
	} else {
		if (bind.inputType == Bind::CONTROLLER_AXIS) {
			setAnalogue(bind.deadZone);
			setBound(0.8f);
		}
		if (bind.inputType == Bind::MOUSE_AXIS) {
			setAnalogue(0.0f);
			setAutoZero();
		}
	}

	switch (bind.inputType) {
	case Bind::MOUSE_AXIS: {
		this->callbacks[0] = event.addObserver(InputSource::MouseAxisEvent::Type, [this](const radix::Event& event) {
						const Vector2f value = ((radix::InputSource::MouseAxisEvent &) event).value;
		
						this->set(value);
					});
		break;
	}
	case Bind::CONTROLLER_AXIS: {
		this->callbacks[1] = event.addObserver(InputSource::ControllerAxisEvent::Type, [this](const radix::Event& event) {
				const int axis = ((radix::InputSource::ControllerAxisEvent &) event).axis;
				const Vector2f value = ((radix::InputSource::ControllerAxisEvent &) event).value;

				if (axis == this->bind.inputCode) {
					this->SubChannel<Vector2f>::set(value);
				}
			});
		break;
	}
	}
}

template class Channel<float>;
template class Channel<Vector2f>;

template class SubChannel<float>;
template class SubChannel<Vector2f>;

}
