/**
 * Created on:  4/25/16
 *     Author:  xing
 */

#pragma once

#include <thread>

#include "utils_cpp/common.h"
#include "utils_cpp/macro_utils.h"
#include "utils_cpp/log.h"
#include "utils_cpp/module.h"

namespace utils
{
class ThreadWrapper final : public Module
{
 public:
  ThreadWrapper() = default;

  explicit ThreadWrapper(std::string &&name) : Module(std::move(name)) {}

  explicit ThreadWrapper(std::thread &&thread)
      : ThreadWrapper{"", std::forward<std::thread &&>(thread)}
  {
  }

  ThreadWrapper(std::string &&name, std::thread &&thread)
      : Module(std::move(name)), routine{std::forward<std::thread &&>(thread)}
  {
    mInf("thread " << routine.get_id() << " attached")
  }

  ~ThreadWrapper() override { detach(); }

  inline void
  attach(std::thread &&th) noexcept
  {
    detach();
    routine = std::forward<std::thread &&>(th);
    mInf("thread " << routine.get_id() << " attached");
  }

  inline ThreadWrapper &
  operator=(std::thread &&th) noexcept
  {
    attach(std::forward<std::thread &&>(th));
    return *this;
  }

  inline void
  detach() noexcept
  {
    if (routine.joinable()) {
      routine.join();
      mInf("thread " << routine.get_id() << " detached");
    }
  }

  inline std::thread::id
  get_id()
  {
    return routine.get_id();
  }

 private:
  UTILS_DISALLOW_COPY_AND_ASSIGN(ThreadWrapper)

  std::thread routine{};
};

}  // namespace utils
