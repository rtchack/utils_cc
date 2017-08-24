/**
 * Created on: 5/26/17
 *     Author: xing
 */

#ifndef BASE_DEV_SUPERVISOR_H
#define BASE_DEV_SUPERVISOR_H

#include <atomic>
#include <mutex>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "base/macro_utils.h"
#include "base/log.h"
#include "base/module.h"
#include "base/thread_wrapper.h"


namespace base{

	/**
	 * Will also init loggers
	 */
	class Supervisor{
	public:

		explicit Supervisor(const std::string &log_file):
				log_file{log_file} {}

		Supervisor(): Supervisor("") {}

		void Start() {
			std::lock_guard<std::mutex> lock{mut};
			if(running){
				std::cout << "Already running" << std::endl;
				return;
			}
			running = true;
			worker.Attach(std::thread(&Supervisor::Entry, this));
		}

		void Stop() {
			std::lock_guard<std::mutex> lock{mut};
			unless(running){
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

			if(pid > 0){
				while(running){
					unless(pid > 0){
						break;
					}

					wait(NULL);
					if(running){
						BASE_SLEEP(seconds(4))
					}
					if(running){
						BASE_SHUTDOWN_LOG
						pid = fork();
					}
				}
			}

			if(pid < 0){
				BASE_RAISE("Invalid pid")
			}else if(!pid){
				if(log_file.size() > 0){
					BASE_INIT_LOG_WITH(log_file)
				}else{
					BASE_INIT_LOG
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

#endif //BASE_DEV_SUPERVISOR_H
