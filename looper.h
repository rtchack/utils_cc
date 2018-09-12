/**
 * Created by xing on 4/12/16.
 */

#pragma once

#include <queue>
#include <list>
#include <cstdint>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>
#include <condition_variable>

#include "macro_utils.h"
#include "module.h"
#include "common.h"
#include "thread_wrapper.h"
#include "timer.h"

namespace utils {

class Looper : public Module {
 public:

  UTILS_DISALLOW_COPY_AND_ASSIGN(Looper)

  /**
   * @return ture to quit the loop, false to continue
   */
  typedef std::function<void()> Task;
  typedef std::list<Task> MsgQueue;


  Looper() : Looper{""} {}

  explicit Looper(const std::string &name) :
      Module{name}, worker{Get_name()} {}

  ~Looper() override {
    Deactivate();
    PutStat();
  }

  /**
   * Post a method
   */
  void Post(const Task &tsk) noexcept {
    Post(tsk, false);
  }

  /**
   * Post a method
   */
  void Post(const Task &task, bool flush) noexcept;

  // TODO: implement
  //void Post(const Task &task, SSTimer::Duration &delay) noexcept;

  /**
   * start loop
   */
  void Activate() noexcept;

  /**
   * stop loop
   */
  void Deactivate() noexcept;

  bool IsActive() const noexcept { return running; }

 protected:
  /**
   * before loop gets started
   */
  virtual void PreActivate() { cDbg("") }

  /**
   * after loop gets started
   */
  virtual void PostActivate() { cDbg("") }

  /**
   * before the quiting of loop
   */
  virtual void PreDeactivate() { cDbg("") }

  /**
   * after loop gets stopped
   */
  virtual void PostDeactivate() { cDbg("") }

  std::string ToString() const noexcept override {
    return Get_name() + stat.ToString();
  }

 private:

  void Entry() noexcept;


  struct Stat {
    inline std::string ToString() const noexcept {
      UTILS_STR_S(16)
      UTILS_STR_ATTR(processed)
      return s;
    }

    uint64_t processed{};
  } stat{};

  MsgQueue msg_queue{};
  ThreadWrapper worker;
  mutable std::mutex run_mut{};
  mutable std::mutex op_mut{};
  volatile bool running{false};
  volatile bool looping{false};
  std::condition_variable cv{};
};

}

