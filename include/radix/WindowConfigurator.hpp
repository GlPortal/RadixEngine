#ifndef RADIX_WINDOW_CONFIGURATOR_HPP
#define RADIX_WINDOW_CONFIGURATOR_HPP

#include <radix/env/Config.hpp>
#include <radix/Window.hpp>

namespace radix {

class WindowConfigurator {
public:
  static void configure(radix::Config &config, radix::Window &window);
};

} /* namespace radix */

#endif /* RADIX_WINDOW_CONFIGURATOR_HPP */
