#ifndef RADIX_CHANNEL_HPP
#define RADIX_CHANNEL_HPP

#include <vector>
#include <array>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/math/Vector2f.hpp>
#include <radix/input/ChannelListener.hpp>
#include <radix/input/ChannelBase.hpp>
#include <radix/input/Bind.hpp>

namespace radix {

template<class T>
class SubChannel;

template<class T>
class Channel: public ChannelBase<T>, ChannelListener {
public:
  Channel() = default;
  Channel(ChannelListener *listener)
    : ChannelBase<T>(listener) {}
  Channel(ChannelListener *listener, const int &id, EventDispatcher &event, const std::vector<Bind> &binds)
    : ChannelBase<T>(listener) {
    this->init(id, event, binds);
  }
  void init(const int &id, EventDispatcher &event, const std::vector<Bind> &binds);
  void reInit(EventDispatcher &event);

  virtual void channelChanged(const int &id) override;

private:
  std::vector<SubChannel<T>> subChannels;

};

template<class T>
class SubChannel: public ChannelBase<T> {
public:
  SubChannel() = default;
  SubChannel(ChannelListener *listener)
    : ChannelBase<T>(listener) {}
  SubChannel(ChannelListener *listener, const int &id, EventDispatcher &event, const Bind &bind)
    : ChannelBase<T>(listener) {
    this->init(id, event, bind);
  }
  void init(const int &id, EventDispatcher &event, const Bind &bind);
  void reInit(EventDispatcher &event);

private:
  void addObservers(EventDispatcher &event);

  Bind bind;

};

}

#endif /* RADIX_INPUT_MANAGER_HPP */
