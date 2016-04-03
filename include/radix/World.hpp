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
#include <radix/core/types/TimeDelta.hpp>
#include <radix/core/event/EventDispatcher.hpp>
#include <radix/material/Material.hpp>
#include <radix/system/System.hpp>
#include <radix/input/InputSource.hpp>

namespace radix {

typedef std::pair<Entity*, Entity*> EntityPair;

class World {
protected:
  Entity *player;

public:
  InputSource &input;

  World(InputSource &input);
  ~World();
  void create();
  void destroy();

  inline double getTime() const {
    return gameTime;
  }
  void update(TDelta dtime);

  Entity& getPlayer();

  Camera camera;
  std::map<int, Material> materials;

  EventDispatcher event;

  EntityManager entities;

  // Convenience
  std::map<std::string, Entity*> entityAliases;
  std::map<std::string, std::vector<Entity*>> entityGroups;
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

  /* =========== */
  /*   Systems   */
  /* =========== */
public:
  using SystemPtrVector = std::vector<std::unique_ptr<System>>;
  struct SystemGraphNode;
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

protected:
  struct SystemRunner {
    std::vector<std::thread> threads;
    std::queue<SystemTypeId> queue;
    std::mutex queueMutex;
    std::condition_variable queueCondVar;
    bool exit;
    TDelta dtime;
    SystemTypeId runCount;
    std::mutex runCountMutex;
    std::condition_variable runCountCondVar;
    SystemRunner(World&);
    ~SystemRunner();
    void threadProc(World&);
  } systemRun;

  SystemPtrVector systems;
  SystemGraphNodeVector systemGraph;
  std::map<std::string, System&> systemsByName;

  void computeSystemOrder();

public:
  using SystemLoopPath = std::stack<SystemTypeId>;
  using SystemLoopVector = std::vector<std::set<SystemTypeId>>;

  struct RunsBeforeCreatesLoopsException : public std::logic_error, SystemLoopVector {
    RunsBeforeCreatesLoopsException(SystemLoopVector &&slv) :
      std::logic_error("Execution graph contain loops"),
      SystemLoopVector(slv) {}
  };
  struct RunsAfterCreatesLoopException : public std::logic_error, SystemLoopPath {
    RunsAfterCreatesLoopException(SystemLoopPath &&slp) :
      std::logic_error("Inserting a System into the execution graph according to"
        "System::runsAfter() would create a loop"),
      SystemLoopPath(slp) {}
  };

  struct SystemAddedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/World:SystemAdded";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    World &world;
    System &system;
    SystemAddedEvent(World &w, System &s) :
      world(w), system(s) {}
  };
  struct SystemRemovedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/World:SystemRemoved";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    World &world;
    System &system;
    SystemRemovedEvent(World &w, System &s) :
      world(w), system(s) {}
  };

  class SystemTransaction final {
  public:
    World &w;

  private:
    friend class World;
    SystemTransaction(World &w) : w(w) {}

  public:
    ~SystemTransaction() {
      w.computeSystemOrder();
    }

    template<class T, typename... ArgsT> void addSystem(ArgsT... args) {
      static_assert(std::is_base_of<System, T>::value, "T must be a System");
      const SystemTypeId stid = System::getTypeId<T>();
      if (w.systems.size() <= stid) {
        w.systems.resize(stid + 1);
      }
      w.systems.at(System::getTypeId<T>()).reset(new T(w, std::forward<ArgsT>(args)...));
      System &sys = *w.systems.at(System::getTypeId<T>());
      w.systemsByName.emplace(std::piecewise_construct,
        std::forward_as_tuple(sys.getName()),
        std::forward_as_tuple(sys));
      w.event.dispatch(SystemAddedEvent(w, *w.systems.at(System::getTypeId<T>())));
    }
    
    template<class T> void removeSystem() {
      System &sys = *w.systems.at(System::getTypeId<T>());
      w.event.dispatch(SystemRemovedEvent(w, sys));
      w.systemsByName.erase(sys.getName());
      w.systemGraph.at(System::getTypeId<T>()).reset(nullptr);
      w.systems.at(System::getTypeId<T>()).reset(nullptr);
    }
  };

  SystemTransaction systemTransact() {
    return SystemTransaction(*this);
  }

  template<class T> T& getSystem() {
    return (T&)*systems.at(System::getTypeId<T>());
  }
  template<class T, typename... ArgsT> void addSystem(ArgsT... args) {
    SystemTransaction st(*this);
    
  }
  template<class T> void removeSystem() {
    SystemTransaction st(*this);
    st.removeSystem<T>();
  }

private:
  double gameTime;
  uint32_t lastUpdateTime;
};

} /* namespace glPortal */

#endif /* RADIX_WORLD_HPP */
