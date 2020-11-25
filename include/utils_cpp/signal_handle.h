/*
 * Created by xing in 2018
 */

#pragma once

#include <functional>

#include "utils_cpp/macro_utils.h"

namespace utils
{
bool
is_running();

void
stop_running();

void
notify_in_running();

void
until_signal(std::function<void()> &&f);

inline void
wait_for_signal()
{
  until_signal([] {});
}

}  // namespace utils
