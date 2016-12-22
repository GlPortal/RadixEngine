#ifndef RADIX_COMPONENT_VIEW_FRAME_HPP
#define RADIX_COMPONENT_VIEW_FRAME_HPP

#include <radix/component/Component.hpp>
#include <radix/data/model/Mesh.hpp>
#include <radix/Camera.hpp>

namespace radix {

class ViewFrame : public Component {
public:
  Mesh mesh;
  Camera view;

  ViewFrame(Entity &ent) :
    Component(ent) {}

  const char* getName() const {
    return "ViewFrame";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver &ar) {
    /// @todo serialization
  }
};

} /* namespace radix */

#endif /* RADIX_COMPONENT_VIEW_FRAME_HPP */
