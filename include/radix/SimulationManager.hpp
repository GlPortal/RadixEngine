#ifndef RADIX_SIMULATION_MANAGER_HPP
#define RADIX_SIMULATION_MANAGER_HPP

#include <atomic>
#include <condition_variable>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <stack>
#include <thread>
#include <typeinfo>
#include <vector>

#include <radix/core/event/Event.hpp>
#include <radix/simulation/Simulation.hpp>

#undef max

namespace radix {

class World;

class SimulationManager {
protected:
  friend class World;
  World &w;
  SimulationManager(World&);

  void dispatchEvent(const Event&);
  void run(TDelta dtime);

public:
  World& world() const {
    return w;
  }

  using Simulation = simulation::Simulation;
  using SimulationPtr = std::unique_ptr<Simulation>;
  struct GraphNode;
  using GraphNodeRef = std::unique_ptr<GraphNode>;
  using Graph = std::vector<GraphNodeRef>;
  struct GraphNode {
    SimulationPtr simulation;
    Graph &graph;

    std::set<GraphNode*> predecessors, successors;

    /* Simulation execution variables */
    std::atomic_uint counter;

    GraphNode(SimulationPtr &&simulation, Graph &graph);
  };

protected:
  struct SimulationRunner {
    std::vector<std::thread> threads;
    std::queue<GraphNode*> queue;
    std::mutex queueMutex;
    std::condition_variable queueCondVar;
    bool exit;
    TDelta dtime;
    unsigned int runCount;
    std::mutex runCountMutex;
    std::condition_variable runCountCondVar;
    SimulationRunner(SimulationManager&);
    ~SimulationRunner();
    void threadProc(int i, SimulationManager&);
  } systemRun;

  Graph graph;

  void computeSimulationOrder();

public:
  using SimulationLoopPath = std::stack<GraphNode*>;

  struct GraphHasLoopsException : public std::logic_error, SimulationLoopPath {
    GraphHasLoopsException(SimulationLoopPath &&slv) :
      std::logic_error("Execution graph contain loops"),
      SimulationLoopPath(slv) {}
  };

  struct SimulationAddedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/SimulationManager:SimulationAdded";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    SimulationManager &sysMan;
    Simulation &system;
    SimulationAddedEvent(SimulationManager &sm, Simulation &s) :
      sysMan(sm), system(s) {}
  };
  struct SimulationRemovedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/SimulationManager:SimulationRemoved";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    SimulationManager &sysMan;
    Simulation &system;
    SimulationRemovedEvent(SimulationManager &sm, Simulation &s) :
      sysMan(sm), system(s) {}
  };

  class Transaction final {
  public:
    SimulationManager &sm;

  private:
    friend class SimulationManager;
    Transaction(SimulationManager &sm) : sm(sm) {}

  public:
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    Transaction(Transaction&&) = default;
    Transaction& operator=(Transaction&&) = default;

    ~Transaction() {
      sm.computeSimulationOrder();
    }

    template<class T, typename... ArgsT> void addSimulation(ArgsT... args) {
      static_assert(std::is_base_of<Simulation, T>::value, "T must be a Simulation");
      static_assert(std::is_constructible<T, decltype(sm.w), ArgsT...>::value,
          "T cannot be constructed with given arguments");
      sm.graph.emplace_back(new GraphNode(
          std::move(std::make_unique<T>(sm.w, std::forward<ArgsT>(args)...)), sm.graph));
      sm.dispatchEvent(SimulationAddedEvent(sm, *sm.graph.back()->simulation));
    }

    // TODO: add a removeSimulation() method
  };

  Transaction transact() {
    return Transaction(*this);
  }

  template<class T> T& findFirstOfType() {
    static_assert(std::is_base_of<Simulation, T>::value, "T must be a Simulation");
    for (const GraphNodeRef &node : graph) {
      Simulation &sim = *node->simulation;
      if (typeid(sim) == typeid(T)) {
        return static_cast<T&>(sim);
      }
    }
    throw std::out_of_range("No Simulation of that type found");
  }
};

} /* namespace radix */

#endif /* RADIX_SIMULATION_MANAGER_HPP */
