#ifndef RADIX_KEY_DECODER_HPP
#define RADIX_KEY_DECODER_HPP

#include <map>

namespace radix {
    
class KeyDecoder {
public:
    static int decodeString(const std::string &keyString);
private:
    static const std::map<std::string, int> lookupTable;
};
    
} /* namespace radix */

#endif
