/*
 * Created by xing on 10/25/18.
 *
 * Copyright (c) 2018
 */

#pragma once

#include <csignal>
#include <atomic>
#include <functional>
#include <iostream>

#include "utilscpp/macro_utils.h"

namespace utils
{
static std::atomic_bool g_running{true};

static void
StopRunning(int sig)
{
  g_running = false;
}

void
UntilSignal(std::function<void()> f)
{
  signal(SIGINT, StopRunning);
  std::cout << "Press Ctrl + C to stop\n" << std::endl;
  while (g_running) {
    f();
  }
}

void
WaitForSignal()
{
  UntilSignal([] { UTILS_SLEEP(milliseconds(1 << 9)) });
}

}  // namespace utils
