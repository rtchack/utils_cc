/**
 * Created by xing on 4/12/16.
 */

#include "utils_cc/looper.h"

namespace ucc
{
void
Looper::post(Task &&tsk, bool flush)
{
  unless(running) {
    mWar("Not running")
    return;
  }
  {
    std::lock_guard<std::mutex> lk{op_mut};
    if (flush) msg_queue.clear();
    msg_queue.push_back(tsk);
  }
  cv.notify_one();
}

void
Looper::deactivate()
{
  {
    std::lock_guard<std::mutex> lk{run_mut};
    unless(running) {
      mInf("is not active");
      return;
    }

    pre_deactivate();

    looping = false;
    cv.notify_one();

    worker.detach();
    running = false;
  }

  post_deactivate();
}

void
Looper::activate()
{
  pre_activate();

  {
    std::lock_guard<std::mutex> lk{run_mut};
    if (running) {
      mInf("is already active");
      return;
    }

    {
      std::lock_guard<std::mutex> op_lk{op_mut};
      msg_queue.clear();
    }

    looping = true;
    worker.attach(std::thread{&Looper::work_entry, this});
    running = true;
  }

  pre_activate();
}

bool
Looper::pull_all_tasks(bool blocking)
{
  Task tsk;
  while (looping) {
    {
      std::unique_lock<std::mutex> lk{op_mut};

      if (blocking) {
        cv.wait(lk, [this] { return !looping || !msg_queue.empty(); });
        if (!looping) {
          return false;
        }

      } else {
        if (msg_queue.empty()) {
          return true;
        }
      }

      tsk = msg_queue.front();
      msg_queue.pop_front();
    }

    ++n_processed;
    tsk();
  }

  return false;
}

void
Looper::work_entry()
{
  pull_all_tasks(true);
  mInf("Quiting")
}

}  // namespace ucc
