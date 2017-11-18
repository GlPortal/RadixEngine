#include <radix/data/screen/Screen.hpp>

#include <radix/World.hpp>
#include <radix/util/KeyDecoder.hpp>

namespace radix {
  
Screen::Screen(World &world, const ScreenConstructor &constructor) :
  color(constructor.color),
  text(constructor.text),
  elements(constructor.elements),
  world(world),
  key(constructor.key) {
    keyPressedHolder = world.event.addObserver(
      radix::InputSource::KeyReleasedEvent::Type, [this] (const radix::Event& event) {
      const int keyCode = ((radix::InputSource::KeyReleasedEvent &) event).key;
      if (key.empty()) {
        return;
      }
      if (keyCode == KeyDecoder::decodeString(key)) {
         remove = true;
      }
  });
}
  
} /* namespace radix */
