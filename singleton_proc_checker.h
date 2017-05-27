/**
 * Created on: 1/11/17
 *     Author: xing
 */

#ifndef RTP_PROTECTOR_SINGLETON_PROC_CHECKER_H
#define RTP_PROTECTOR_SINGLETON_PROC_CHECKER_H

#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <unistd.h>

#include "base/common.h"

using namespace base;

class SingletonProcChecker{
public:
	SingletonProcChecker() = delete;

	SingletonProcChecker(const std::string &name):
			fd{socket(AF_UNIX, SOCK_STREAM, 0)} {
		BASE_RISE_UNLESS(fd)

		const auto full_path = std::string{"/tmp/"} + name;

		sockaddr_un addr{};
		addr.sun_family = AF_UNIX;
		strncpy(addr.sun_path, full_path.c_str(), sizeof(addr.sun_path));
		auto len = sizeof(addr);

		unless(connect(fd, (sockaddr *)&addr, len)){
			BASE_RISE(full_path << " already running")
		}

		if(unlink(addr.sun_path)){
			unless(errno == ENOENT){
				BASE_RISE(full_path << " unlink: " << std::strerror(errno))
			}
		}
		if(bind(fd, (sockaddr *)&addr, len)){
			BASE_RISE(full_path << " bind: " << std::strerror(errno))
		}

		if(listen(fd, 4)){
			BASE_RISE(full_path << " listen: " << std::strerror(errno))
		}
	}

	~SingletonProcChecker(){
		close(fd);
	}

private:
	BASE_DISALLOW_COPY_AND_ASSIGN(SingletonProcChecker)

	int fd;
};

#endif //RTP_PROTECTOR_SINGLETON_PROC_CHECKER_H
