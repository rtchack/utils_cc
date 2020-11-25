/*
 * Created by xing in 2018
 */

#include "utils_cpp/singleton_guard.h"

#ifdef UNIX

#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <unistd.h>

#endif

namespace utils
{
#ifdef CONF_UNIX

SingletonGuard::SingletonGuard(const std::string &name)
    : fd{socket(AF_UNIX, SOCK_STREAM, 0)}
{
  UTILS_RAISE_UNLESS(fd)

  const auto full_path = std::string{"/tmp/"} + name;

  sockaddr_un addr{};
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, full_path.c_str(), sizeof(addr.sun_path));
  auto len = sizeof(addr);

  unless(connect(fd, (sockaddr *)&addr, len))
  {
    UTILS_RAISE(full_path << " already running")
  }

  if (unlink(addr.sun_path)) {
    unless(errno == ENOENT)
    {
      UTILS_RAISE(full_path << " unlink: " << std::strerror(errno))
    }
  }
  if (bind(fd, (sockaddr *)&addr, len)) {
    UTILS_RAISE(full_path << " bind: " << std::strerror(errno))
  }

  if (listen(fd, 4)) {
    UTILS_RAISE(full_path << " listen: " << std::strerror(errno))
  }

  std::cout << "Listening on " << full_path << std::endl;
}

SingletonGuard::~SingletonGuard() { close(fd); }

#endif

#ifdef CONF_WIN32

SingletonGuard::SingletonGuard(const std::string &name) : fd{0}
{
  NOT_IMPLEMENTED
}

SingletonGuard::~SingletonGuard()
{
  NOT_IMPLEMENTED
}

#endif

}  // namespace utils
