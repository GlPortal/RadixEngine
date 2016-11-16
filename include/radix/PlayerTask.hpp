#ifndef RADIX_PLAYERTASK_HPP
#define RADIX_PLAYERTASK_HPP

#include <functional>
#include <string>
#include <list>

#include <radix/BaseGame.hpp>


namespace radix {

class PlayerTask {
public:
  using Task = std::function<void(BaseGame *game, TDelta dtime)>;

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

class PlayerTriggerTask : public PlayerTask {
public:
  PlayerTriggerTask();
  std::string getName();
};

class PlayerMoveTask : public PlayerTask {
public:
  PlayerMoveTask();
  std::string getName();
};

} /* namespace radix */
#endif /* RADIX_PLAYERTASK_HPP */
