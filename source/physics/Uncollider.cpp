#include <radix/physics/Uncollider.hpp>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <radix/physics/PhysicsHelper.hpp>
#include <radix/system/PhysicsSystem.hpp>
#include <radix/World.hpp>

#include <ciso646>

namespace radix {

std::list<btCollisionObject*> Uncollider::volumes;
std::unordered_set<std::pair<btCollisionObject*, btCollisionObject*>>
  Uncollider::collisonPairExclusions;

Uncollider::Uncollider(World &w) :
  world(w) {
}

bool Uncollider::isPointInUncollideVolume(const btVector3 &p) {
  if (volumes.size() > 0) {
    for (btCollisionObject *volume : volumes) {
      if (PhysicsHelper::pointInVolume(p, volume)) {
        return true;
      }
    }
  }
  return false;
}

void Uncollider::beforePhysicsStep() {
  if (volumes.size() > 0) {
    for (btCollisionObject *volume : volumes) {
      btVector3 ext = ((btBoxShape*)volume)->getHalfExtentsWithoutMargin();
      world.systems.get<PhysicsSystem>().physicsWorld->getDebugDrawer()->drawBox(-ext, ext,
        volume->getWorldTransform(), btVector3(1, .5, 0));
    }
  }
}

void Uncollider::addCollisonPairExclusion(btCollisionObject *a, btCollisionObject *b) {
  collisonPairExclusions.emplace(a, b);
  collisonPairExclusions.emplace(b, a);
}

void Uncollider::removeCollisonPairExclusion(btCollisionObject *a, btCollisionObject *b) {
  collisonPairExclusions.erase(collisonPairExclusions.find(std::make_pair(a, b)));
  collisonPairExclusions.erase(collisonPairExclusions.find(std::make_pair(a, b)));
}

bool Uncollider::needBroadphaseCollision(btBroadphaseProxy *proxy0,btBroadphaseProxy *proxy1)
  const {
  bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
  collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

  btCollisionObject *colObj0 = (btCollisionObject*)proxy0->m_clientObject;
  btCollisionObject *colObj1 = (btCollisionObject*)proxy1->m_clientObject;
  auto it1 = collisonPairExclusions.find(std::make_pair(colObj0, colObj1));
  if (it1 != collisonPairExclusions.end()) {
    return false;
  }

  auto it2 = collisonPairExclusions.find(std::make_pair(colObj1, colObj0));
  if (it2 != collisonPairExclusions.end()) {
    return false;
  }

  return collides;
}

void Uncollider::nearCallback(btBroadphasePair &collisionPair,
  btCollisionDispatcher &dispatcher, const btDispatcherInfo &dispatchInfo) {

  btCollisionObject *colObj0 = (btCollisionObject*)collisionPair.m_pProxy0->m_clientObject;
  btCollisionObject *colObj1 = (btCollisionObject*)collisionPair.m_pProxy1->m_clientObject;
  
  if (dispatcher.needsCollision(colObj0, colObj1)) {
    btCollisionObjectWrapper obj0Wrap(0, colObj0->getCollisionShape(),
      colObj0, colObj0->getWorldTransform(), -1, -1);
    btCollisionObjectWrapper obj1Wrap(0, colObj1->getCollisionShape(),
      colObj1, colObj1->getWorldTransform(), -1, -1);

    if (not collisionPair.m_algorithm) {
      collisionPair.m_algorithm = dispatcher.findAlgorithm(&obj0Wrap, &obj1Wrap, nullptr, BT_CONTACT_POINT_ALGORITHMS);
    }

    if (collisionPair.m_algorithm) {
      btManifoldResult contactPointResult(&obj0Wrap, &obj1Wrap);
      if (dispatchInfo.m_dispatchFunc == btDispatcherInfo::DISPATCH_DISCRETE) {
        collisionPair.m_algorithm->processCollision(&obj0Wrap, &obj1Wrap, dispatchInfo,
          &contactPointResult);
        for (int i = 0; i < contactPointResult.getPersistentManifold()->getNumContacts(); ++i) {
          const btManifoldPoint &pt = contactPointResult.getPersistentManifold()->
            getContactPoint(i);
          const btVector3 &cp = pt.getPositionWorldOnA();
          if (isPointInUncollideVolume(cp)) {
            contactPointResult.getPersistentManifold()->removeContactPoint(i--);
          }
        }
      } else {
        btScalar toi = collisionPair.m_algorithm->calculateTimeOfImpact(colObj0, colObj1,
          dispatchInfo, &contactPointResult);
        if (dispatchInfo.m_timeOfImpact > toi) {
          dispatchInfo.m_timeOfImpact = toi;
        }
      }
    }
  }
}

} /* namespace radix */
