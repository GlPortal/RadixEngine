#ifndef RADIX_PLAYERTASK_HPP
#define RADIX_PLAYERTASK_HPP

#include <functional>
#include <string>
#include <list>

#include <radix/World.hpp>

namespace radix {

class PlayerTask {
public:
  using Task = std::function<void(World &world)>;

  PlayerTask() {};
  virtual ~PlayerTask() {};

  int id;
  Task task;
  std::list<PlayerTask *> blackList;

  template<typename T>
  inline void addBlacklist() {
    static_assert(std::is_base_of<PlayerTask, T>::value, "T must be a PlayerTask");
    T *result = new T;
    blackList.push_back(result);
  }

  virtual std::string getName() = 0;
};

class PlayerTestTask : public PlayerTask {
public:
  PlayerTestTask();
  std::string getName();
};

} /* namespace radix */
#endif /* RADIX_PLAYERTASK_HPP */
