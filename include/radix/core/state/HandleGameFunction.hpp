#ifndef RADIX_HANDLEGAMEFUNCTION_HPP
#define RADIX_HANDLEGAMEFUNCTION_HPP

#include <functional>

namespace radix {

class BaseGame;

using HandleGameFunction = std::function<void (BaseGame &game)>;

} /* namespace radix */

#endif /* RADIX_HANDLEGAMEFUNCTION_HPP */
