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

  explicit ThreadWrapper(const std::string &name) : Module(name) {}

  explicit ThreadWrapper(std::thread &&thread)
      : ThreadWrapper{"", std::forward<std::thread &&>(thread)}
  {
  }

  ThreadWrapper(const std::string &name, std::thread &&thread)
      : Module(name), routine{std::forward<std::thread &&>(thread)}
  {
    std::cout << "Thread " << routine.get_id() << " attached" << std::endl;
  }

  ~ThreadWrapper() override { detach(); }

  inline void
  attach(std::thread &&th) noexcept
  {
    detach();
    routine = std::forward<std::thread &&>(th);
    std::cout << "Thread " << routine.get_id() << " attached" << std::endl;
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
      std::cout << "Thread " << routine.get_id() << " detached" << std::endl;
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
