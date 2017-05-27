/**
 * Created on: 5/26/17
 *     Author: xing
 */

#ifndef BASE_DEV_SUPERVISOR_H
#define BASE_DEV_SUPERVISOR_H

#include <atomic>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "base/macro_utils.h"
#include "base/file_wrapper.h"
#include "base/log.h"
#include "base/helper.h"
#include "base/module.h"
#include "base/thread_wrapper.h"


namespace base{

	class Supervisor: public Module{
	public:
		Supervisor():
				Module(std::string("Superv#") + std::to_string(getpid())) {}

		void Start() {
			std::lock_guard<std::mutex> lock{mut};
			if(running){
				cInf("Already running")
				return;
			}
			running = true;
			worker.Attach(std::thread(&Supervisor::Entry, this));
		}

		void Stop() {
			std::lock_guard<std::mutex> lock{mut};
			unless(running){
				cInf("Not running")
				return;
			}
			running = false;
			worker.Reset();
		}

		virtual void RunInDescendant() = 0;

	private:

		void Entry() {
			auto pid = fork();
			int stat_file{-1};
			int offset{-1};

			if(pid > 0){
				while(running){
					unless(pid > 0){
						break;
					}

					BASE_SLEEP(seconds(2))

					if(stat_file < 0){
						stat_file = StatFile(pid);
						BASE_RISE_VERBOSE_IF(stat_file < 0, strerror(errno))
						offset = StatFileOffset(stat_file);
						BASE_RISE_UNLESS(offset > 0)
					}

					const auto stat = ReadAt(stat_file, offset);
					switch(stat){
						case 'Z':{
							close(stat_file);
							stat_file = -1;
							wait();
							pid = fork();
							break;
						}
						default:{
							cDbg("Stat of " << pid << ": " << (char)stat)
							break;
						}
					}
				}
			}

			unless(stat_file < 0){
				close(stat_file);
			}

			if(pid < 0){
				BASE_RISE_LOCATED("Invalid pid " << pid)
			}else if(!pid){
				RunInDescendant();
			}

			cInf("Wait for " << pid)
			wait();
			cInf("Out of running")
		}

		static int ReadAt(int file, int offset) {
			BASE_RISE_VERBOSE_IF(lseek(file, offset, SEEK_SET) < 0,
			                     strerror(errno))
			char c;
			BASE_RISE_VERBOSE_IF(read(file, &c, 1) < 0, strerror(errno))
			return c;
		}

		static int StatFileOffset(int stat_file) {
			BASE_RISE_VERBOSE_IF(lseek(stat_file, 0, SEEK_SET) < 0,
			                     strerror(errno))
			std::string tmp{};
			char c;
			int pos{};
			for(uint32_t space_count{0}; space_count < 2; ++pos){
				BASE_RISE_VERBOSE_IF(read(stat_file, &c, 1) < 0,
				                     strerror(errno))
				BASE_RISE_IF(c == EOF)

				tmp += c;
				if(c == ' '){
					++space_count;
				}
			}

			BASE_RISE_VERBOSE_IF(read(stat_file, &c, 1) < 0,
			                     strerror(errno))
			BASE_RISE_IF(c == EOF)
			tmp += c;
			lDbg(tmp)
			return c == EOF ? -1 : pos;
		}

		static inline int StatFile(__pid_t pid) {
			char file_name[128];
			snprintf(file_name, 128, "/proc/%u/stat", pid);
			lDbg("File: " << file_name)
			return open(file_name, O_RDONLY);
		}

		ThreadWrapper worker{};
		std::mutex mut{};
		bool running{false};
	};

}

#endif //BASE_DEV_SUPERVISOR_H
