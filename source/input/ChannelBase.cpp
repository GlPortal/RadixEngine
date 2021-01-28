#include <radix/input/ChannelBase.hpp>

#include <radix/core/math/Math.hpp>
#include <radix/core/math/Vector2f.hpp>
#include <radix/input/ChannelListener.hpp>

#include <cstdlib>

namespace radix {

template <class T>
ChannelBase<T>::ChannelBase()
    : listeners(),
    id(0),
    bound(0),
    sensitivity(1.0f),
    hasBound(false),
    isDigital(false),
    autoZero(false),
    alwaysNotifyListener(false),
    actPoint(0),
    value() {}

template <class T>
ChannelBase<T>::ChannelBase(ChannelListener<T>* listener)
    : listeners(1, listener),
    id(0),
    bound(0),
    sensitivity(1.0f),
    hasBound(false),
    isDigital(false),
    autoZero(false),
    alwaysNotifyListener(false),
    actPoint(0),
    value() {}

template <class T>
void ChannelBase<T>::addListener(ChannelListener<T>* listener) {
  for (auto* mListener: listeners) {
    if (listener == mListener) {
      return;
    }
  }

  listeners.push_back(listener);
}

template <class T>
void ChannelBase<T>::setDigital(const float &actPoint) {
  this->actPoint = actPoint;
  isDigital = true;
}

template <class T>
void ChannelBase<T>::setAnalogue(const float &deadZone) {
  this->deadZone = deadZone;
  isDigital = false;
}

template <class T>
void ChannelBase<T>::setBound(const float &bound) {
  this->bound = bound;
  hasBound = true;
}

template <class T>
void ChannelBase<T>::set(T newValue) {
  if (isDigital) {
    newValue = std::abs(newValue) >= actPoint ? 1.0f : 0.0f;
  } else {
    newValue = std::abs(newValue) >= deadZone ? newValue : 0.0f;

    if (hasBound) {
      newValue = std::abs(newValue) >= bound ? 1.0f : newValue;
    }
    newValue *= sensitivity;
  }

  if (value != newValue || alwaysNotifyListener) {
    value = newValue;

    notifyListeners();
  } else {
    return;
  }

  if (autoZero) {
    value = 0.0f;
  }
}

template <>
void ChannelBase<Vector2f>::set(Vector2f newValue) {
  if (isDigital) {
    newValue.x = newValue.x >= actPoint ? 1.0f : 0.0f;
    newValue.y = newValue.y >= actPoint ? 1.0f : 0.0f;
  } else {
    float length = newValue.length();
    if (deadZone > 0) {
      if (length <= deadZone) {
        newValue = Vector2f::ZERO;
      } else {
        newValue *= (length - deadZone) / (length * (1 - deadZone));
      }
    }
    if (hasBound) {
      newValue.x = newValue.x >= bound ? 1.0f : newValue.x;
      newValue.y = newValue.y >= bound ? 1.0f : newValue.y;

      newValue.x = newValue.x <= -bound ? -1.0f : newValue.x;
      newValue.y = newValue.y <= -bound ? -1.0f : newValue.y;
    }

    newValue *= sensitivity;
  }

  if (alwaysNotifyListener || !newValue.fuzzyEqual(value, 0.0001)) {
    value = newValue;

    notifyListeners();
  } else {
    return;
  }
}

template <class T>
void ChannelBase<T>::notifyListeners() {
  for (auto* listener: this->listeners) {
    listener->channelChanged(value, this->id);
  }
}

template class ChannelBase<float>;
template class ChannelBase<bool>;
template class ChannelBase<Vector2f>;

}
