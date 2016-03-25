#include <stdexcept>
#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <chrono>

#include <radix/env/Environment.hpp>
#include <radix/World.hpp>
#include <radix/system/System.hpp>
#include <radix/system/PlayerSystem.hpp>
#include <radix/system/PhysicsSystem.hpp>

class TestSystem : public radix::System {
public:
  TestSystem(radix::World &w) : radix::System(w) {}
  ~TestSystem();

  const char* getName() const {
    return "TestSystem";
  }

  TypeId getTypeId() const {
    return System::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  bool runsAfter(const System &s) {
    return s.getTypeId() == System::getTypeId<radix::PlayerSystem>() or
           s.getTypeId() == System::getTypeId<radix::PhysicsSystem>();
  }
  /*bool runsBefore(const System &s) {
    return strcmp(s.getName(), "TestSystem2") == 0;
  }*/

  void update(float dtime) {
    // std::cout << "run TestSystem" << std::endl;
  }
};

class TestSystem2 : public radix::System {
public:
  TestSystem2(radix::World &w) : radix::System(w) {}
  ~TestSystem2();

  const char* getName() const {
    return "TestSystem2";
  }

  TypeId getTypeId() const {
    return System::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  bool runsBefore(const System &s) {
    return s.getTypeId() == System::getTypeId<TestSystem>();
  }

  void update(float dtime) {
    // std::cout << "run TestSystem2" << std::endl;
  }
};

class RunBeforeSystem : public radix::System {
public:
  RunBeforeSystem(radix::World &w) : radix::System(w) {}
  ~RunBeforeSystem();

  const char* getName() const {
    return "RunBeforeSystem";
  }

  TypeId getTypeId() const {
    return System::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  bool runsBefore(const System &s) {
    return s.getTypeId() == System::getTypeId<radix::PhysicsSystem>();
  }
  /*bool runsAfter(const System &s) {
    return strcmp(s.getName(), "TestSystem") == 0;
  }*/

  void update(float dtime) {
    // std::cout << "run RunBeforeSystem" << std::endl;
  }
};

template<int I> class TinySystem : public radix::System {
public:
  std::string name;
  TinySystem(radix::World &w) : radix::System(w), name("TinySystem") {
    name += std::to_string(I);
  }
  ~TinySystem();

  const char* getName() const {
    return name.c_str();
  }

  TypeId getTypeId() const {
    return System::getTypeId<typename std::remove_reference<decltype(*this)>::type>();
  }

  bool runsBefore(const System &s) {
    return strncmp(s.getName(), "TinySystem", 10) == 0 and
           atoi(s.getName() + 10) > I;
  }

  void update(float dtime) {
    // std::cout << "run TinySystem" << I << std::endl;
  }
};

template<int I> void add(radix::World &wld) {
  wld.addSystem<TinySystem<I>>();
  add<I-1>(wld);
}

template<> void add<1>(radix::World &wld) {
  wld.addSystem<TinySystem<1>>();
}

int main(const int argc, char *argv[]) {
  if (argc <= 1) {
    std::cout << "Please pass data dir as argument" << std::endl;
  }
  radix::Environment::setDataDir(argv[1]);
  radix::Environment::init();
  radix::World wld;
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  { radix::World::SystemTransaction st = wld.systemTransact();
    wld.addSystem<radix::PlayerSystem>();
    wld.addSystem<radix::PhysicsSystem>();
    wld.addSystem<TestSystem>();
    wld.addSystem<TestSystem2>();
    wld.addSystem<RunBeforeSystem>();
    add<10>(wld);
    start = std::chrono::high_resolution_clock::now();
    // Here, st gets destructed, changes committed (i.e. system graph recomputed)
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end-start;
  std::cout << "Graph sorting: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " ms\n";

  constexpr int runs = 10000;
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < runs; ++i) {
    wld.update(0.016);
  }
  end = std::chrono::high_resolution_clock::now();
  diff = end-start;
  std::cout << "Graph running: " <<
    std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1000>>>(diff).count() /
      runs << " ms/run (" << runs << " runs, " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << "ms total)\n";

  return 0;
}
