//
// Created by geert on 9/25/16.
//

#ifndef HANDLEGAMEFUNCTION_HPP
#define HANDLEGAMEFUNCTION_HPP

class Game;

namespace radix {
typedef std::function<void (Game &game)> HandleGameFunction;
} /* namespace radix */

#endif //GLPORTAL_HANDLEGAMEFUNCTION_HPP
