#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <map>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <radix/material/Material.hpp>
#include <radix/Camera.hpp>
#include <radix/EntityManager.hpp>
#include <radix/physics/CollisionDispatcher.hpp>
#include <radix/scene/Screen.hpp>
#include <radix/scene/Terminal.hpp>

namespace radix {

typedef std::pair<Entity*, Entity*> EntityPair;
class World;

class Scene {
public:
  struct Physics {
    Scene &parent;
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    CollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;
    btGhostPairCallback *gpCallback;
    Physics(Scene &parent);
    ~Physics();
    void setGravity(float x, float y, float z);

    Physics(const Physics&) = delete;
    Physics(Physics&&) = delete;
  } physics;

  World *world;
  Camera camera;
  std::map<int, Material> materials;
  EntityManager entities;
  std::map<std::string, std::vector<EntityPair>> entityPairs;
  std::unique_ptr<Screen> screen;
  std::unique_ptr<Terminal> terminal;
  Entity *player;
  // FIXME: remove us!
  Entity *start;
  Entity *end;

  Scene();
  Scene(Scene&) = delete;
  Scene(Scene&&) = delete;
};

} /* namespace radix */

#endif /* SCENE_HPP */
