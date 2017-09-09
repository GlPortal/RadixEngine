#include <radix/simulation/Physics.hpp>

#include <typeinfo>
#include <utility>

#include <radix/entities/Player.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/entities/traits/RigidBodyTrait.hpp>
#include <radix/physics/CollisionDispatcher.hpp>
#include <radix/physics/GhostPairCallback.hpp>
#include <radix/physics/Uncollider.hpp>
#include <radix/util/BulletUserPtrInfo.hpp>
#include <radix/World.hpp>

using namespace radix::entities;

static const char *Tag = "sim::Physics";

namespace radix {
namespace simulation {

static ContactAddedCallback prevContactAddedCallback;
static ContactProcessedCallback prevContactProcessedCallback;
static ContactDestroyedCallback prevContactDestroyedCallback;

std::unordered_set<CollisionInfo, CollisionInfoHash, CollisionInfoEqual> Physics::collisions;
Physics *instance; //< TODO: get rid of this

Physics::Physics(World &world, BaseGame *game) :
  Simulation(world),
  filterCallback(nullptr),
  game(game),
  broadphase(new btDbvtBroadphase),
  collisionConfiguration(new btDefaultCollisionConfiguration()),
  dispatcher(new CollisionDispatcher(collisionConfiguration)),
  solver(new btSequentialImpulseConstraintSolver),
  physicsWorld(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
                                           collisionConfiguration)),
  gpCallback(new GhostPairCallback) {
  broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(gpCallback.get());
  filterCallback = new Uncollider(world);
  //physWorld->getPairCache()->setOverlapFilterCallback(filterCallback);
  dispatcher->setNearCallback(Uncollider::nearCallback);
  physicsWorld->setGravity(btVector3(0, -9.80665, 0));

  prevContactAddedCallback = gContactAddedCallback;
  gContactAddedCallback = reinterpret_cast<ContactAddedCallback>
      (&Physics::contactAddedCallback);
  prevContactProcessedCallback = gContactProcessedCallback;
  gContactProcessedCallback = reinterpret_cast<ContactProcessedCallback>
      (&Physics::contactProcessedCallback);
  prevContactDestroyedCallback = prevContactDestroyedCallback;
  gContactDestroyedCallback = reinterpret_cast<ContactDestroyedCallback>
      (&Physics::contactDestroyedCallback);

  instance = this;
}

Physics::~Physics() {
  delete filterCallback;
}

void Physics::setDebugDraw(btIDebugDraw *debugDraw) {
  physicsWorld->setDebugDrawer(debugDraw);
}

btIDebugDraw* Physics::getDebugDraw() const {
  return physicsWorld->getDebugDrawer();
}

void Physics::update(TDelta timeDelta) {
  filterCallback->beforePhysicsStep();
  physicsWorld->stepSimulation(timeDelta.sec(), 10);
  for (Entity &entity : world.entityManager) {
    RigidBodyTrait *rbtp = dynamic_cast<RigidBodyTrait*>(&entity);
    if (rbtp and rbtp->body and not rbtp->body->isStaticObject()) {
      btTransform btTform;
      rbtp->body->getMotionState()->getWorldTransform(btTform);
      entity.privSetPosition(btTform.getOrigin());
      entity.privSetOrientation(btTform.getRotation());
    }
  }

  ContactPlayerCallback callback;
  physicsWorld->contactTest(world.getPlayer().obj, callback);
  //checkCollisions();
}


bool Physics::contactAddedCallback(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0,
    int partId0, int index0, const btCollisionObjectWrapper *colObj1, int partId1, int index1) {
  // is const_casting really safe?
  CollisionInfo pair(const_cast<btCollisionObject*>(colObj0->getCollisionObject()),
                     const_cast<btCollisionObject*>(colObj1->getCollisionObject()));
  auto inserted = collisions.insert(std::move(pair));
  if (inserted.second) {
    World &world = util::getBtPtrInfo(colObj0->getCollisionObject()).entity->world;
    world.event.dispatch(CollisionAddedEvent(*inserted.first, world));
    cp.m_userPersistentData = (void*) &*inserted.first;
  }
  return true; /* the return value is ignored */
}

bool Physics::contactProcessedCallback(btManifoldPoint &cp, void *body0, void *body1) {
  return true;
}

bool Physics::contactDestroyedCallback(void *userPersistentData) {
  auto it = collisions.find(*reinterpret_cast<CollisionInfo*>(userPersistentData));
  if (it != collisions.end()) {
    CollisionInfo info = *it; // Copy because we're going to erase it from the container
    collisions.erase(it);
    World &world = util::getBtPtrInfo(info.body0).entity->world;
    world.event.dispatch(CollisionRemovedEvent(info, world));
  }
  return true;
}

void Physics::checkCollisions() {
  if (!collisions.empty()) {
    std::vector<CollisionInfo*> toRemove;
    /*vector because we need to avoid modifying a data structure when we iterate over it */

    for (CollisionInfo info : collisions) {
      bool remove = true;
      CheckCollisionCallback callback(&remove);
      if (info.body0 && info.body1) {
        physicsWorld->contactPairTest(info.body0, info.body1, callback);
      }
      if (remove) {
        toRemove.push_back(&info);
      }
    }

    if (!toRemove.empty()) {
      for (CollisionInfo *info : toRemove) {
        CollisionInfo infoCopy = *info;
        collisions.erase(*info);
        world.event.dispatch(CollisionRemovedEvent(infoCopy, world));
      }
    }
  }
}

std::string Physics::CollisionAddedEvent::debugStringRepr() const {
  const Entity *ent0 = util::getBtPtrInfo(info.body0).entity;
  const Entity *ent1 = util::getBtPtrInfo(info.body1).entity;
  return ent0->className() + "(#" + std::to_string(ent0->id) + ",\"" + ent0->name() + "\") x " +
         ent1->className() + "(#" + std::to_string(ent1->id) + ",\"" + ent1->name() + "\")";
}

std::string Physics::CollisionRemovedEvent::debugStringRepr() const {
  const Entity *ent0 = util::getBtPtrInfo(info.body0).entity;
  const Entity *ent1 = util::getBtPtrInfo(info.body1).entity;
  return ent0->className() + "(#" + std::to_string(ent0->id) + ",\"" + ent0->name() + "\") x " +
         ent1->className() + "(#" + std::to_string(ent1->id) + ",\"" + ent1->name() + "\")";
}

} /* namespace simulation */
} /* namespace radix */
