#include <radix/physics/GhostPairCallback.hpp>

#include <bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <radix/env/Util.hpp>
#include <radix/util/BulletUserPtrInfo.hpp>
#include <radix/util/BulletGhostPairCallbacks.hpp>

static const char *Tag = "GhostPairCB";

namespace radix {

using GPCallbacks = util::BulletGhostPairCallbacks;

btBroadphasePair* GhostPairCallback::addOverlappingPair(
    btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) {
  btCollisionObject *colObj0 = reinterpret_cast<btCollisionObject*>(proxy0->m_clientObject);
  btCollisionObject *colObj1 = reinterpret_cast<btCollisionObject*>(proxy1->m_clientObject);
  btGhostObject *ghost0 = btGhostObject::upcast(colObj0);
  btGhostObject *ghost1 = btGhostObject::upcast(colObj1);
  if (ghost0) {
    ghost0->addOverlappingObjectInternal(proxy1, proxy0);
    const GPCallbacks &gpcallbacks = util::getBtPtrAs<GPCallbacks>(ghost0);
    if (gpcallbacks.onEnter) {
      gpcallbacks.onEnter(GPCallbacks::CallbackParams(gpcallbacks, ghost0, colObj1));
    }
  }
  if (ghost1) {
    ghost1->addOverlappingObjectInternal(proxy0, proxy1);
    const GPCallbacks &gpcallbacks = util::getBtPtrAs<GPCallbacks>(ghost1);
    if (gpcallbacks.onEnter) {
      gpcallbacks.onEnter(GPCallbacks::CallbackParams(gpcallbacks, ghost1, colObj0));
    }
  }
  return 0;
}

void* GhostPairCallback::removeOverlappingPair(
    btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1, btDispatcher *dispatcher) {
  btCollisionObject *colObj0 = reinterpret_cast<btCollisionObject*>(proxy0->m_clientObject);
  btCollisionObject *colObj1 = reinterpret_cast<btCollisionObject*>(proxy1->m_clientObject);
  btGhostObject *ghost0 = btGhostObject::upcast(colObj0);
  btGhostObject *ghost1 = btGhostObject::upcast(colObj1);
  if (ghost0) {
    ghost0->removeOverlappingObjectInternal(proxy1, dispatcher, proxy0);
    const GPCallbacks &gpcallbacks = util::getBtPtrAs<GPCallbacks>(ghost0);
    if (gpcallbacks.onExit) {
      gpcallbacks.onExit(GPCallbacks::CallbackParams(gpcallbacks, ghost0, colObj1));
    }
  }
  if (ghost1) {
    ghost1->removeOverlappingObjectInternal(proxy0, dispatcher, proxy1);
    const GPCallbacks &gpcallbacks = util::getBtPtrAs<GPCallbacks>(ghost1);
    if (gpcallbacks.onExit) {
      gpcallbacks.onExit(GPCallbacks::CallbackParams(gpcallbacks, ghost1, colObj0));
    }
  }
  return 0;
}

void GhostPairCallback::removeOverlappingPairsContainingProxy(
    btBroadphaseProxy *proxy, btDispatcher *dispatcher) {
  btAssert(0);
  //need to keep track of all ghost objects and call them here
  //m_hashPairCache->removeOverlappingPairsContainingProxy(proxy0,dispatcher);
}

} /* namespace radix */
