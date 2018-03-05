#ifndef RADIX_CHANNEL_LISTENER_HPP
#define RADIX_CHANNEL_LISTENER_HPP

namespace radix {

template<class T>
class ChannelListener {
public:
  virtual void channelChanged(T newValue, const int &id) = 0;

};

class Vector2f;

typedef ChannelListener<bool> 		DigitalChannelListener;
typedef ChannelListener<Vector2f> 	VectorChannelListener;

}

#endif /* RADIX_INPUT_MANAGER_HPP */
