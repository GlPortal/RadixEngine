#include <radix/entities/StaticModel.hpp>

namespace radix {
namespace entities {

StaticModel::StaticModel(const CreationParams &cp) :
  Entity(cp) {

}

StaticModel::~StaticModel() {
}

int StaticModel::getCollisionFlags() const {
  return btCollisionObject::CF_STATIC_OBJECT;
}

int StaticModel::getCollisionGroup() const {
  return btBroadphaseProxy::StaticFilter;
}

int StaticModel::getCollisionMask() const {
  return btBroadphaseProxy::DefaultFilter |
         btBroadphaseProxy::CharacterFilter |
         btBroadphaseProxy::DebrisFilter;
}

} /* namespace entities */
} /* namespace radix */
