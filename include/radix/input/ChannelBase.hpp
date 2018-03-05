#ifndef RADIX_CHANNEL_BASE_HPP
#define RADIX_CHANNEL_BASE_HPP

#include <list>

namespace radix {

template <class T>
class ChannelListener;

template <class T>
class ChannelBase {
public:
  ChannelBase();
  ChannelBase(ChannelListener<T> *listener);

  void setId(const int &id) { this->id = id; }
  void addListener(ChannelListener<T> *listener);
  void setDigital(const float &actPoint);
  void setAnalogue(const float &deadZone);
  void setBound(const float& bound);
  void setSensitivity(const float& sensitivity) { this->sensitivity = sensitivity; }
  void setAutoZero() { autoZero = true; }
  void setAlwaysNotifyListener() { alwaysNotifyListener = true; }
  void set(T newValue);
  int getId() const { return id; }
  T get() const { return value; }

protected:
  void notifyListeners();

  std::list<ChannelListener<T>*> listeners;
  int id;
  float bound, sensitivity;
  bool hasBound, isDigital, autoZero, alwaysNotifyListener;
  union {
    float deadZone, actPoint;
  };
  T value;

};

}

#endif /* RADIX_INPUT_MANAGER_HPP */
