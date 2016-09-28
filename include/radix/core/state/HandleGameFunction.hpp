//
// Created by geert on 9/25/16.
//

#ifndef RADIX_HANDLEGAMEFUNCTION_HPP
#define RADIX_HANDLEGAMEFUNCTION_HPP

namespace radix {

class BaseGame;

typedef std::function<void (BaseGame &game)> HandleGameFunction;

} /* namespace radix */

#endif /* RADIX_HANDLEGAMEFUNCTION_HPP */
