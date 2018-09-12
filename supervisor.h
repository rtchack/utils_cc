/**
 * Created on: 5/26/17
 *     Author: xing
 */

#pragma once

#include <atomic>
#include <mutex>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "macro_utils.h"
#include "log.h"
#include "module.h"
#include "thread_wrapper.h"


namespace utils {

/**
 * Will also init loggers
 */
class Supervisor {
 public:

  explicit Supervisor(const std::string &log_file) :
      log_file{log_file} {}

  Supervisor() : Supervisor("") {}

  ~Supervisor() {
    Stop();
  }

  void Start() {
    std::lock_guard<std::mutex> lock{mut};
    if (running) {
      std::cout << "Already running" << std::endl;
      return;
    }
    running = true;
    worker.Attach(std::thread(&Supervisor::Entry, this));
  }

  void Stop() {
    std::lock_guard<std::mutex> lock{mut};
    unless(running) {
      std::cout << "Not running" << std::endl;
      return;
    }
    running = false;
    worker.Detach();
  }

  virtual void RunInDescendant() = 0;

 private:

  void Entry() {
    auto pid = fork();

    if (pid > 0) {
      while (running) {
        unless(pid > 0) {
          break;
        }

        wait(NULL);
        if (running) {
          UTILS_SLEEP(seconds(4))
        }
        if (running) {
          UTILS_SHUTDOWN_LOG
          pid = fork();
        }
      }
    }

    if (pid < 0) {
      UTILS_RAISE("Invalid pid")
    } else if (!pid) {
      if (log_file.size() > 0) {
        UTILS_INIT_LOG_WITH(log_file)
      } else {
        UTILS_INIT_LOG
      }
      std::cout << getpid() << " Go into RunInDescendant" << std::endl;
      RunInDescendant();
      return;
    }

    std::cout << "Out of running" << std::endl;
  }

  std::string log_file{};
  ThreadWrapper worker{};
  std::mutex mut{};
  bool running{false};
};

}

