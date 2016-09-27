//
// Created by geert on 9/25/16.
//

#ifndef HANDLEGAMEFUNCTION_HPP
#define HANDLEGAMEFUNCTION_HPP

namespace radix {

class BaseGame;

typedef std::function<void (BaseGame &game)> HandleGameFunction;

} /* namespace radix */

#endif //GLPORTAL_HANDLEGAMEFUNCTION_HPP
