#ifndef RADIX_SYSTEM_MANAGER_HPP
#define RADIX_SYSTEM_MANAGER_HPP

#include <condition_variable>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <stack>
#include <thread>
#include <vector>

#include <radix/core/event/Event.hpp>
#include <radix/system/System.hpp>

#undef max

namespace radix {

/*
 * THREAD SAFETY UTILITIES
 */

/*
 * Declares a variable of type #type, and of name #name.
 * Along with that, it declares a mutex of name #name##Mutex.
 * This macro should be used to declare variables that must
 * remain thread safe (almost always since this is a multi-threaded engine).
 * It should not be used when declaring variables on the stack, for (hopefully)
 * obvious reasons. It is recommended to use this in conjunction with
 * RADIX_DECLARE_MUTEX_GETTER, to keep code clean.
 */

#define RADIX_DECLARE_WITH_MUTEX(type, name) type name; std::mutex name##Mutex

/*
 * Declares a getter method for variable declared with RADIX_DECLARE_WITH_MUTEX.
 */

#define RADIX_DECLARE_SAFE_GETTER(type, name)               \
inline type get##name() {                                   \
  name##Mutex.lock();                                       \
  return name;                                              \
}                                                           \
                                                            \
inline void relinquish##name##Mutex() {                     \
  name##Mutex.unlock();                                     \
}                                                           \


class World;

class SystemManager {
protected:
  friend class World;
  World &w;
  SystemManager(World&);

  void dispatchEvent(const Event&);
  void run(TDelta dtime);

public:
  World& world() const {
    return w;
  }

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
    unsigned int counter;
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
    SystemRunner(SystemManager&);
    ~SystemRunner();
    void threadProc(SystemManager&);
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
    static constexpr StaticEventTypeName TypeName = "radix/SystemManager:SystemAdded";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    SystemManager &sysMan;
    System &system;
    SystemAddedEvent(SystemManager &sm, System &s) :
      sysMan(sm), system(s) {}
  };
  struct SystemRemovedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/SystemManager:SystemRemoved";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    SystemManager &sysMan;
    System &system;
    SystemRemovedEvent(SystemManager &sm, System &s) :
      sysMan(sm), system(s) {}
  };

  class Transaction final {
  public:
    SystemManager &sm;

  private:
    friend class SystemManager;
    Transaction(SystemManager &sm) : sm(sm) {}

  public:
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    Transaction(Transaction&&) = default;
    Transaction& operator=(Transaction&&) = default;

    ~Transaction() {
      sm.computeSystemOrder();
    }

    template<class T, typename... ArgsT> void addSystem(ArgsT... args) {
      static_assert(std::is_base_of<System, T>::value, "T must be a System");
      const SystemTypeId stid = System::getTypeId<T>();
      if (sm.systems.size() <= stid) {
        sm.systems.resize(stid + 1);
      }
      sm.systems.at(System::getTypeId<T>()).reset(new T(sm.w, std::forward<ArgsT>(args)...));
      System &sys = *sm.systems.at(System::getTypeId<T>());
      sm.systemsByName.emplace(std::piecewise_construct,
        std::forward_as_tuple(sys.getName()),
        std::forward_as_tuple(sys));
      sm.dispatchEvent(SystemAddedEvent(sm, *sm.systems.at(System::getTypeId<T>())));
    }

    template<class T> void removeSystem() {
      System &sys = *sm.systems.at(System::getTypeId<T>());
      sm.dispatchEvent(SystemRemovedEvent(sm, sys));
      sm.systemsByName.erase(sys.getName());
      sm.systemGraph.at(System::getTypeId<T>()).reset(nullptr);
      sm.systems.at(System::getTypeId<T>()).reset(nullptr);
    }
  };

  Transaction transact() {
    return Transaction(*this);
  }

  template<class T> T& get() {
    // We're casting down the class hierarchy, maybe use dynamic_cast instead ?
    // But unless the systems collection break, it should always be ok.
    return static_cast<T&>(*systems.at(System::getTypeId<T>()));
  }

  template<class T, typename... ArgsT> void add(ArgsT... args) {
    Transaction st(*this);
    st.addSystem<T>(std::forward(args)...);
  }

  template<class T> void remove() {
    Transaction st(*this);
    st.removeSystem<T>();
  }
};

} /* namespace radix */

#endif /* RADIX_SYSTEM_MANAGER_HPP */
