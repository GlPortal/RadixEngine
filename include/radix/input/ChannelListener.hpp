#ifndef RADIX_CHANNEL_LISTENER_HPP
#define RADIX_CHANNEL_LISTENER_HPP

namespace radix {

class ChannelListener {
public:
	virtual void channelChanged(const int &id) = 0;

};

}

#endif /* RADIX_INPUT_MANAGER_HPP */
