/*
 * Created by xing in 2020
 */

#include "utils_cpp/signal_handle.h"
#include "utils_cpp/macro_utils.h"

#include <csignal>
#include <atomic>
#include <iostream>
#include <condition_variable>

namespace utils {
static std::atomic_bool g_running{true};
static std::condition_variable running_cv{};

bool
is_running()
{
  return g_running;
}

void
stop_running()
{
  g_running = false;
  notify_in_running();
}

void
notify_in_running()
{
  running_cv.notify_all();
}

static void
handle_sig(int sig)
{
  UTILS_UNUSED(sig);
  stop_running();
}

void
until_signal(std::function<void()> &&f)
{
  signal(SIGINT, handle_sig);
  std::cout << "Press Ctrl + C to stop\n" << std::endl;

  std::mutex mut{};
  while (g_running) {
    std::unique_lock<std::mutex> lk{mut};
    running_cv.wait(lk, [] { return true; });
    f();
  }
}

}  // namespace utils