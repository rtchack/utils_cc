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
	class Supervisor: public Module{
	public:

		Supervisor(const std::string &name): Module(name) {}

		Supervisor(): Supervisor("Superv") {}

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
				BASE_INIT_LOG_WITH(Get_name())
				while(running){
					unless(pid > 0){
						break;
					}

					wait(NULL);
					if(running){
						BASE_SHUTDOWN_LOG
						pid = fork();
					}
				}
			}

			if(pid < 0){
				BASE_INIT_LOG_WITH(Get_name() + std::string{".invalid"})
				BASE_RAISE("Invalid pid")
			}else if(!pid){
				BASE_INIT_LOG_WITH(Get_name() + std::string{".child"})
				cInf(getpid() << " Go into RunInDescendant")
				RunInDescendant();
				return;
			}

			cInf("Out of running")
		}

		ThreadWrapper worker{};
		std::mutex mut{};
		bool running{false};
	};

}

#endif //BASE_DEV_SUPERVISOR_H
