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

#include "utils_cc/macro_utils.h"
#include "utils_cc/module.h"
#include "utils_cc/common.h"
#include "utils_cc/thread_wrapper.h"
#include "utils_cc/time.h"

namespace ucc
{
class Looper : public Module
{
 public:
  /**
   * @return ture to quit the loop, false to continue
   */
  typedef std::function<void()> Task;
  typedef std::list<Task> MsgQueue;

  Looper() : Looper{""} {}

  explicit Looper(std::string &&name)
      : Module{std::move(name)}, worker{std::string{get_name()}}
  {
  }

  ~Looper() override { deactivate(); }

  /**
   * Post a method
   */
  void
  post(Task &&tsk)
  {
    post(std::move(tsk), false);
  }

  /**
   * Post a method
   */
  void
  post(Task &&task, bool flush);

  // TODO: implement
  // void post(const Task &task, SSTimer::Duration &delay);

  /**
   * start loop
   */
  void
  activate();

  /**
   * stop loop
   */
  void
  deactivate();

  bool
  is_active() const noexcept
  {
    return running;
  }

 protected:
  /**
   * before loop gets started
   */
  virtual void
  pre_activate()
  {
    mDbg("")
  }

  /**
   * after loop gets started
   */
  virtual void
  post_activate()
  {
    mDbg("")
  }

  /**
   * before the quiting of loop
   */
  virtual void
  pre_deactivate()
  {
    mDbg("")
  }

  /**
   * after loop gets stopped
   */
  virtual void
  post_deactivate(){mDbg("")}

  std::string to_s() const noexcept override
  {
    return get_name() + ", n_processed {" + std::to_string(n_processed);
  }

  /**
   * PUll and run tasks.
   * @param blocking Should block or not if there's not tasks.
   * @return false if looper is stopped, true otherwise.
   */
  bool
  pull_all_tasks(bool blocking);

  virtual void
  work_entry();

 private:
  ThreadWrapper worker;
  MsgQueue msg_queue{};
  mutable std::mutex run_mut{};
  mutable std::mutex op_mut{};
  std::condition_variable cv{};
  uint64_t n_processed{};
  volatile bool looping{false};
  volatile bool running{false};

  UCC_DISALLOW_COPY_AND_ASSIGN(Looper)
};

}  // namespace ucc
