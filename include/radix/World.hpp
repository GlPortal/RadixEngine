#ifndef RADIX_WORLD_HPP
#define RADIX_WORLD_HPP

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include <radix/Camera.hpp>
#include <radix/EntityManager.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/material/Material.hpp>
#include <radix/system/System.hpp>

namespace radix {

typedef std::pair<Entity*, Entity*> EntityPair;

class World {
protected:
  Entity *player;

  struct SystemRunner {
    std::vector<std::thread> threads;
    std::queue<SystemTypeId> queue;
    std::mutex queueMutex;
    std::condition_variable queueCondVar;
    bool exit;
    double dtime;
    SystemTypeId runCount;
    std::mutex runCountMutex;
    std::condition_variable runCountCondVar;
    SystemRunner(World&);
    ~SystemRunner();
    void threadProc(World&);
  } systemRun;

public:
  using SystemLoopPath = std::stack<SystemTypeId>;
  using SystemLoopVector = std::vector<std::set<SystemTypeId>>;

  struct RunsBeforeCreatesLoopsException : public std::logic_error, SystemLoopVector {
    RunsBeforeCreatesLoopsException(SystemLoopVector &&slv) :
      std::logic_error("Execution graph created with System::runsBefore() contain loops"),
      SystemLoopVector(slv) {}
  };
  struct RunsAfterCreatesLoopException : public std::logic_error, SystemLoopPath {
    RunsAfterCreatesLoopException(SystemLoopPath &&slp) :
      std::logic_error("Inserting a System into the execution graph according to"
        "System::runsAfter() would create a loop"),
      SystemLoopPath(slp) {}
  };

  void computeSystemOrder();

  struct SystemAddedEvent : public Event {
    static constexpr StaticEventType Type = "radix/World/SystemAdded";
    const EventType getType() const {
      return Type;
    }

    World &world;
    System &system;
    SystemAddedEvent(World &w, System &s) :
      world(w), system(s) {}
  };
  struct SystemRemovedEvent : public Event {
    static constexpr StaticEventType Type = "radix/World/SystemRemoved";
    const EventType getType() const {
      return Type;
    }

    World &world;
    System &system;
    SystemRemovedEvent(World &w, System &s) :
      world(w), system(s) {}
  };


  Camera camera;
  std::map<int, Material> materials;

  EventDispatcher event;

  EntityManager entities;
  std::map<std::string, std::vector<EntityPair>> entityPairs;

  /**
   * Gets the reference to the entity with specified ID.
   * @throws std::out_of_range if no entity with this ID is found.
   */
  inline Entity& getEntityById(EntityId id) {
    return entities.getById(id);
  }

  /**
   * Gets the reference to the entity with specified name.
   * @throws std::out_of_range if no entity with this name is found.
   */
  inline Entity& getEntityByName(const std::string &name) {
    return entities.getByName(name);
  }


  /// @todo make publicly read-only
  struct SystemGraphNode;
  using SystemPtrVector = std::vector<std::unique_ptr<System>>;
  using SystemGraphNodeVector = std::vector<std::unique_ptr<SystemGraphNode>>;
  struct SystemGraphNode {
    const SystemTypeId system;
    SystemPtrVector &systems;
    SystemGraphNodeVector &graph;

    std::set<SystemTypeId> predecessors, successors;

    /* Tarjan's strongly connected components algothm data */
    SystemTypeId index, lowlink;
    bool onStack;
    static constexpr decltype(index) indexUndef = std::numeric_limits<decltype(index)>::max();
    static constexpr decltype(lowlink) lowlinkUndef = std::numeric_limits<decltype(lowlink)>::max();

    /* System execution variables */
    uint counter;
    std::mutex counterMut;

    SystemGraphNode(SystemTypeId system, SystemPtrVector &systems, SystemGraphNodeVector &graph);
  };
  SystemPtrVector systems;
  SystemGraphNodeVector systemGraph;
  std::map<std::string, System*> systemsByName;
  template<class T, typename... ArgsT> void addSystem(ArgsT... args) {
    static_assert(std::is_base_of<System, T>::value, "T must be a System");
    const SystemTypeId stid = System::getTypeId<T>();
    if (systems.size() <= stid) {
      systems.resize(stid + 1);
    }
    systems.at(System::getTypeId<T>()).reset(new T(*this, std::forward<ArgsT>(args)...));
    System &sys = *systems.at(System::getTypeId<T>());
    systemsByName.emplace(std::piecewise_construct,
      std::forward_as_tuple(sys.getName()),
      std::forward_as_tuple(&sys));
    event.dispatch(SystemAddedEvent(*this, *systems.at(System::getTypeId<T>())));
  }
  template<class T> T& getSystem() {
    return (T&)*systems.at(System::getTypeId<T>());
  }
  template<class T> void removeSystem() {
    System &sys = *systems.at(System::getTypeId<T>());
    event.dispatch(SystemRemovedEvent(*this, sys));
    systemsByName.erase(sys.getName());
    systemGraph.at(System::getTypeId<T>()).reset(nullptr);
    systems.at(System::getTypeId<T>()).reset(nullptr);
  }


  World();
  ~World();
  void create();
  void destroy();

  inline double getTime() const {
    return gameTime;
  }
  void update(double dtime);

  Entity& getPlayer();

private:
  double gameTime;
  uint32_t lastUpdateTime;
};

} /* namespace glPortal */

#endif /* RADIX_WORLD_HPP */
