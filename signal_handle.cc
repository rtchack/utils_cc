/*
 * Created by xing in 2020
 */

#include "utils_cpp/signal_handle.h"

#include <csignal>
#include <atomic>
#include <iostream>
#include <condition_variable>

namespace utils
{
static std::atomic_bool g_running{true};
static std::condition_variable running_cv{};

bool
is_running()
{
  return g_running;
}

void
stop_running(int sig)
{
  g_running = false;
  running_cv.notify_all();
}

void
until_signal(std::function<void()> f)
{
  signal(SIGINT, stop_running);
  std::cout << "Press Ctrl + C to stop\n" << std::endl;

  std::mutex mut{};
  while (g_running) {
    std::unique_lock<std::mutex> lk{mut};
    running_cv.wait(lk, [] { return true; });
    f();
  }
}

}  // namespace utils