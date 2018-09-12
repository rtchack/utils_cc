/**
 * Created on: 1/11/17
 *     Author: xing
 */

#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <unistd.h>

#include "common.h"

namespace utils {

class SingletonProcChecker {
 public:

  UTILS_DISALLOW_COPY_AND_ASSIGN(SingletonProcChecker)

  SingletonProcChecker() = delete;

  explicit
  SingletonProcChecker(
      const std::string
      &name) :
      fd
          {socket(AF_UNIX, SOCK_STREAM, 0)} {
    UTILS_RAISE_UNLESS(fd)

    const auto full_path = std::string{"/tmp/"}
                           + name;

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, full_path.c_str(), sizeof(addr.sun_path));
    auto len = sizeof(addr);

    unless(connect(fd, (sockaddr *) &addr, len)) {
      UTILS_RAISE(full_path << " already running")
    }

    if (unlink(addr.sun_path)) {
      unless(errno == ENOENT) {
        UTILS_RAISE(full_path << " unlink: " << std::strerror(errno))
      }
    }
    if (bind(fd, (sockaddr *) &addr, len)) {
      UTILS_RAISE(full_path << " bind: " << std::strerror(errno))
    }

    if (listen(fd, 4)) {
      UTILS_RAISE(full_path << " listen: " << std::strerror(errno))
    }

    std::cout << "Listening on " << full_path << std::endl;
  }

  ~SingletonProcChecker() {
    close(fd);
  }

 private:

 UTILS_DIRECT_READER(int, fd);
};

}

