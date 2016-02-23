#include <radix/scene/Scene.hpp>

#include <radix/component/Transform.hpp>
#include <radix/component/Health.hpp>
#include <radix/component/SoundSource.hpp>
#include <radix/component/SoundListener.hpp>
#include <radix/component/RigidBody.hpp>
#include <radix/component/Player.hpp>

namespace radix {

Scene::Physics::Physics(Scene &parent) :
  parent(parent),
  broadphase(new btDbvtBroadphase()),
  collisionConfiguration(new btDefaultCollisionConfiguration()),
  dispatcher(new CollisionDispatcher(collisionConfiguration)),
  solver(new btSequentialImpulseConstraintSolver),
  world(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration)),
  gpCallback(new btGhostPairCallback) {
  broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(gpCallback);
}

void Scene::Physics::setGravity(float x, float y, float z) {
  world->setGravity(btVector3(x, y, z));
}

Scene::Physics::~Physics() {
  delete gpCallback;
  delete world;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;
}

Scene::Scene() :
  physics(*this),
  world(nullptr),
  entities(*this),
  start(nullptr),
  end(nullptr) {
  player = &entities.create();
  player->addComponent<Transform>().setPosition(Vector3f(2.5, 1, 5));
  player->addComponent<Health>();
  player->addComponent<SoundSource>();
  player->addComponent<SoundListener>();
  player->addComponent<Player>();
  screen   = std::make_unique<Screen>("GlPortal", "Hit enter to start.");
  terminal = std::make_unique<Terminal>();
}

} /* namespace radix */
