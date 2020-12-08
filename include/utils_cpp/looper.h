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

#include "utils_cpp/macro_utils.h"
#include "utils_cpp/module.h"
#include "utils_cpp/common.h"
#include "utils_cpp/thread_wrapper.h"
#include "utils_cpp/time.h"

namespace utils
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
  post(Task &&tsk) noexcept
  {
    post(std::move(tsk), false);
  }

  /**
   * Post a method
   */
  void
  post(Task &&task, bool flush) noexcept;

  // TODO: implement
  // void post(const Task &task, SSTimer::Duration &delay) noexcept;

  /**
   * start loop
   */
  void
  activate() noexcept;

  /**
   * stop loop
   */
  void
  deactivate() noexcept;

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
    return get_name() + stat.to_s();
  }

  virtual void
  work_entry() noexcept;

 private:
  UTILS_DISALLOW_COPY_AND_ASSIGN(Looper)

  struct Stat {
    inline std::string
    to_s() const noexcept
    {
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

}  // namespace utils
