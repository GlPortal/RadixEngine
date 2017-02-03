#ifndef RADIX_EMITTER_HPP
#define RADIX_EMITTER_HPP

#include <radix/component/Component.hpp>

namespace radix {

class Emitter : public Component {
public:
  Emitter();

  const char* getName() const {
    return "Emitter";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver&);
private:
  int particles;
  bool infinite;
};

} /* namespace radix */

#endif RADIX_EMITTER_HPP
