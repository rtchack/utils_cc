/**
 * Created on:  4/25/16
 *     Author:  xing
 */

#pragma once

#include <thread>

#include "common.h"
#include "macro_utils.h"
#include "log.h"
#include "module.h"

namespace utils {

class ThreadWrapper final : public Module {
 public:

  UTILS_DISALLOW_COPY_AND_ASSIGN(ThreadWrapper)

  ThreadWrapper() = default;

  explicit ThreadWrapper(const std::string &name) :
      Module(name) {}

  explicit ThreadWrapper(std::thread &&thread) :
      ThreadWrapper{"", std::forward<std::thread &&>(thread)} {}

  ThreadWrapper(const std::string &name, std::thread &&thread) :
      Module(name), routine{std::forward<std::thread &&>(thread)} {
    std::cout << "Thread " << routine.get_id() << " attached" << std::endl;
  }

  ~ThreadWrapper() override {
    Detach();
  }

  inline void Attach(std::thread &&th) noexcept {
    Detach();
    routine = std::forward<std::thread &&>(th);
    std::cout << "Thread " << routine.get_id() << " attached" << std::endl;
  }

  inline ThreadWrapper &operator=(std::thread &&th) noexcept {
    Attach(std::forward<std::thread &&>(th));
    return *this;
  }

  inline void Detach() noexcept {
    if (routine.joinable()) {
      routine.join();
      std::cout << "Thread " << routine.get_id() << " detached" << std::endl;
    }
  }

  inline std::thread::id GetId() {
    return routine.get_id ();
  }

 private:

  std::thread routine{};
};

}

