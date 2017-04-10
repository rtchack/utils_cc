/**
 * Created on:  4/25/16
 *     Author:  xing
 */

#ifndef CPPBASE_THREAD_WRAPPER_H_H
#define CPPBASE_THREAD_WRAPPER_H_H

#include <thread>

#include "base/common.h"
#include "base/macro_utils.h"
#include "base/log.h"
#include "base/module.h"

namespace base{

	class ThreadWrapper final: public Module{
	public:
		ThreadWrapper() = default;

		ThreadWrapper(const std::string &name): Module(name) {}

		ThreadWrapper(std::thread &&thread):
				ThreadWrapper{"", std::forward<std::thread &&> (thread)} {}

		ThreadWrapper(const std::string &name, std::thread &&thread):
				Module(name), routine{std::forward<std::thread &&>(thread)} {
			cInf(routine.get_id() << " attached")
		};

		~ThreadWrapper() {Reset();};

		inline void Attach(std::thread &&th) noexcept {
			Reset();
			routine = std::forward<std::thread &&>(th);
			cInf(routine.get_id() << " attached")
		}

		inline void operator=(std::thread &&th) noexcept {
			Attach(std::forward<std::thread &&>(th));}

		inline void Reset() noexcept {
			if(routine.joinable()){
				cInf(routine.get_id() << " detached")
				routine.join();
			}
		}

	private:
		BASE_DISALLOW_COPY_AND_ASSIGN(ThreadWrapper)

		std::thread routine{};
	};

}

#endif //CPPBASE_THREAD_WRAPPER_H_H
