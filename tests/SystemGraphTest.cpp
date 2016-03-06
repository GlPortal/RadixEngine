#include <stdexcept>
#include <string>
#include <cstdio>
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
    return strcmp(s.getName(), "PlayerSystem") == 0 or
           strcmp(s.getName(), "PhysicsSystem") == 0;
  }
  /*bool runsBefore(const System &s) {
    return strcmp(s.getName(), "TestSystem2") == 0;
  }*/

  void update(float dtime) {}
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
    return strcmp(s.getName(), "TestSystem") == 0;
  }

  void update(float dtime) {}
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
    return strcmp(s.getName(), "PhysicsSystem") == 0;
  }
  /*bool runsAfter(const System &s) {
    return strcmp(s.getName(), "TestSystem") == 0;
  }*/

  void update(float dtime) {}
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

  void update(float dtime) {}
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
  wld.addSystem<radix::PlayerSystem>();
  wld.addSystem<radix::PhysicsSystem>();
  wld.addSystem<TestSystem>();
  wld.addSystem<TestSystem2>();
  wld.addSystem<RunBeforeSystem>();
  add<10>(wld);
  
  auto start = std::chrono::high_resolution_clock::now();
  wld.computeSystemOrder();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end-start;
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " ms\n";
  
  return 0;
}
