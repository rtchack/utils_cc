/**
 * Created by xing on 4/12/16.
 */

#include "utils_cpp/looper.h"

namespace utils
{
void
Looper::post(Task &&tsk, bool flush) noexcept
{
  unless(running) { mWar("Not running") return; }
  {
    std::lock_guard<std::mutex> lk{op_mut};
    if (flush) msg_queue.clear();
    msg_queue.push_back(tsk);
  }
  cv.notify_one();
}

void
Looper::deactivate() noexcept
{
  {
    std::lock_guard<std::mutex> lk{run_mut};
    unless(running)
    {
      mInf("is not active");
      return;
    }

    pre_deactivate();

    looping = false;
    post([] { ; }, true);
    worker.detach();
    running = false;
  }

  post_deactivate();
}

void
Looper::activate() noexcept
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

void
Looper::work_entry() noexcept
{
  Task tsk;
  while (looping) {
    {
      std::unique_lock<std::mutex> lk{op_mut};
      cv.wait(lk, [this] { return !msg_queue.empty(); });
      // if(msg_queue.empty()) continue;
      tsk = msg_queue.front();
      msg_queue.pop_front();
    }

    ++stat.processed;
    tsk();
  }

  mInf("Quiting")
}

}  // namespace utils
