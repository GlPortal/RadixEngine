#ifndef RADIX_CHANNEL_HPP
#define RADIX_CHANNEL_HPP

#include <list>
#include <vector>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/math/Vector2f.hpp>
#include <radix/input/ChannelListener.hpp>
#include <radix/input/ChannelBase.hpp>
#include <radix/input/Bind.hpp>

namespace radix {

template<class T>
class SubChannel;

class DigitalChannel: public ChannelBase<bool>, ChannelListener<float> {
public:
  DigitalChannel() = default;
  DigitalChannel(ChannelListener<bool> *listener)
    : ChannelBase<bool>(listener) {}
  void init(const int &id, EventDispatcher &event, const std::vector<Bind> &binds);
  void reInit(EventDispatcher &event);

  void channelChanged(float newValue, const int &id) override;

private:
  std::list<SubChannel<float>> subChannels;

};

class VectorChannel: public ChannelBase<Vector2f>, ChannelListener<Vector2f> {
public:
  VectorChannel() = default;
  VectorChannel(ChannelListener<Vector2f> *listener)
    : ChannelBase<Vector2f>(listener) {}
  void init(const int &id, EventDispatcher &event, const std::vector<Bind> &binds);
  void reInit(EventDispatcher &event);

  void channelChanged(Vector2f newValue, const int &id) override;

private:
  std::list<SubChannel<Vector2f>> subChannels;

};

template<class T>
class SubChannel: public ChannelBase<T> {
public:
  SubChannel() = default;
  SubChannel(ChannelListener<T> *listener)
    : ChannelBase<T>(listener) {}
  void init(const int &id, EventDispatcher &event, const Bind &bind);
  void reInit(EventDispatcher &event);

private:
  void addObservers(EventDispatcher &event);

  Bind bind;

};

}

#endif /* RADIX_INPUT_MANAGER_HPP */
