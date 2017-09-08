#ifndef RADIX_ENTITIES_VIEW_FRAME_HPP
#define RADIX_ENTITIES_VIEW_FRAME_HPP

#include <radix/Entity.hpp>

#include <radix/data/model/Mesh.hpp>
#include <radix/Camera.hpp>

namespace radix {
namespace entities {

class ViewFrame : public Entity {
public:
  Mesh mesh;
  Camera view;

  using Entity::Entity;

  std::string fullClassName() const override {
    return "radix/entities/ViewFrame";
  }
  std::string className() const override {
    return "ViewFrame";
  }
};

} /* namespace entities */
} /* namespace radix */

#endif /* RADIX_ENTITIES_VIEW_FRAME_HPP */
