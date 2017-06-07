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
#include "base/log.h"
#include "base/module.h"
#include "base/thread_wrapper.h"


namespace base{

	class Supervisor: public Module{
	public:

		Supervisor(const std::string &name):
				Module(name + std::to_string(getpid())) {}

		Supervisor(): Supervisor("Superv") {}

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
			worker.Detach();
		}

		virtual void RunInDescendant() = 0;

	private:

		void Entry() {
			auto pid = fork();
			int stat_file{-1};
			int offset{-1};

			std::cout << "pid " << pid << std::endl;

			if(pid > 0){
				while(running){
					unless(pid > 0){
						break;
					}

					BASE_SLEEP(seconds(4))

					if(stat_file < 0){
						stat_file = StatFile(pid);
						BASE_RISE_VERBOSE_IF(stat_file < 0, strerror(errno))
						offset = StatFileOffset(stat_file);
						BASE_RISE_UNLESS(offset > 0)
					}

					const auto c = ReadAt(stat_file, offset);
					switch(c){
						case 'Z':{
							stat.Succ();
							close(stat_file);
							stat_file = -1;
							wait();
							if(running){
								pid = fork();
							}
							break;
						}
						default:{
							stat.Total();
							cDbg("Stat of " << pid << ": " << (char)c)
							break;
						}
					}
				}
			}

			unless(stat_file < 0){
				close(stat_file);
			}

			if(pid < 0){
				BASE_RISE("Invalid pid")
			}else if(!pid){
				cInf(getpid() << " Go into RunInDescendant")
				RunInDescendant();
				return;
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
