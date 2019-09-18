#include <radix/WindowConfigurator.hpp>

namespace radix {
void WindowConfigurator::configure(radix::Config &config, radix::Window &window){
  if (config.isLoaded()){
    window.setIgnoreGlVersion(config.getIgnoreGlVersion());
    window.setAntialiasLevel(config.getAntialiasLevel());
  }

}
} /* namespace radix */

